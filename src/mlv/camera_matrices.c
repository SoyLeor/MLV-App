/* This may be the code I'm least proud of,
 * I know there is a simpler/better solution */

/* THIS IS NOT USED! I got lost and stopped at the point when I 
 * realised some matrices were row-wise and others column-wise */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "video_mlv.h"
#include "raw.h"
#include "mlv.h"
#include "../matrix/matrix.h"

struct cam_matrices {
    char * camera;
    int32_t ColorMatrix1[18];
    int32_t ColorMatrix2[18];
    int32_t ForwardMatrix1[18];
    int32_t ForwardMatrix2[18];
};

static uint8_t diagonal_flip[9] = { 0, 3, 6, 1, 4, 7, 2, 5, 8 };
#define diag_flip(X) diagonal_flip[(X)]

/* credits to Andy600 for gleaning these from Adobe DNG converter */
static struct cam_matrices cam_matrices[] =
{
    { /* 0 */
        "Canon EOS 5D Mark III",
        { 7234, 10000, -1413, 10000, -600, 10000, -3631, 10000, 11150, 10000, 2850, 10000, -382, 10000, 1335, 10000, 6437, 10000 },
        { 6722, 10000, -635, 10000, -963, 10000, -4287, 10000, 12460, 10000, 2028, 10000, -908, 10000, 2162, 10000, 5668, 10000 },
        { 7868, 10000, 92, 10000, 1683, 10000, 2291, 10000, 8615, 10000, -906, 10000, 27, 10000, -4752, 10000, 12976, 10000 },
        { 7637, 10000, 805, 10000, 1201, 10000, 2649, 10000, 9179, 10000, -1828, 10000, 137, 10000, -2456, 10000, 10570, 10000 }
    },
    { /* 1 */
        "Canon EOS 5D Mark II",
        { 5309, 10000, -229, 10000, -336, 10000, -6241, 10000, 13265, 10000, 3337, 10000, -817, 10000, 1215, 10000, 6664, 10000 },
        { 4716, 10000, 603, 10000, -830, 10000, -7798, 10000, 15474, 10000, 2480, 10000, -1496, 10000, 1937, 10000, 6651, 10000 },
        { 8924, 10000, -1041, 10000, 1760, 10000, 4351, 10000, 6621, 10000, -972, 10000, 505, 10000, -1562, 10000, 9308, 10000 },
        { 8924, 10000, -1041, 10000, 1760, 10000, 4351, 10000, 6621, 10000, -972, 10000, 505, 10000, -1562, 10000, 9308, 10000 }
    },
    { /* 2 */
        "Canon EOS 7D",
        { 11620, 10000, -6350, 10000, 5, 10000, -2558, 10000, 10146, 10000, 2813, 10000, 24, 10000, 858, 10000, 6926, 10000 },
        { 6844, 10000, -996, 10000, -856, 10000, -3876, 10000, 11761, 10000, 2396, 10000, -593, 10000, 1772, 10000, 6198, 10000 },
        { 5445, 10000, 3536, 10000, 662, 10000, 1106, 10000, 10136, 10000, -1242, 10000, -374, 10000, -3559, 10000, 12184, 10000 },
        { 7415, 10000, 1533, 10000, 695, 10000, 2499, 10000, 9997, 10000, -2497, 10000, -22, 10000, -1933, 10000, 10207, 10000 }
    },
    { /* 3 */
        "Canon EOS 6D",
        { 7546, 10000, -1435, 10000, -929, 10000, -3846, 10000, 11488, 10000, 2692, 10000, -332, 10000, 1209, 10000, 6370, 10000 },
        { 7034, 10000, -804, 10000, -1014, 10000, -4420, 10000, 12564, 10000, 2058, 10000, -851, 10000, 1994, 10000, 5758, 10000 },
        { 7763, 10000, 65, 10000, 1815, 10000, 2364, 10000, 8351, 10000, -715, 10000, -59, 10000, -4228, 10000, 12538, 10000 },
        { 7464, 10000, 1044, 10000, 1135, 10000, 2648, 10000, 9173, 10000, -1820, 10000, 113, 10000, -2154, 10000, 10292, 10000 }
    },
    { /* 4 */
        "Canon EOS 70D",
        { 7546, 10000, -1435, 10000, -929, 10000, -3846, 10000, 11488, 10000, 2692, 10000, -332, 10000, 1209, 10000, 6370, 10000 },
        { 7034, 10000, -804, 10000, -1014, 10000, -4420, 10000, 12564, 10000, 2058, 10000, -851, 10000, 1994, 10000, 5758, 10000 },
        { 7763, 10000, 65, 10000, 1815, 10000, 2364, 10000, 8351, 10000, -715, 10000, -59, 10000, -4228, 10000, 12538, 10000 },
        { 7464, 10000, 1044, 10000, 1135, 10000, 2648, 10000, 9173, 10000, -1820, 10000, 113, 10000, -2154, 10000, 10292, 10000 }
    },
    { /* 5 */
        "Canon EOS 60D",
        { 7428, 10000, -1897, 10000, -491, 10000, -3505, 10000, 10963, 10000, 2929, 10000, -337, 10000, 1242, 10000, 6413, 10000 },
        { 6719, 10000, -994, 10000, -925, 10000, -4408, 10000, 12426, 10000, 2211, 10000, -887, 10000, 2129, 10000, 6051, 10000 },
        { 7550, 10000, 645, 10000, 1448, 10000, 2138, 10000, 8936, 10000, -1075, 10000, -5, 10000, -4306, 10000, 12562, 10000 },
        { 7286, 10000, 1385, 10000, 972, 10000, 2600, 10000, 9468, 10000, -2068, 10000, 93, 10000, -2268, 10000, 10426, 10000 }
    },
    { /* 6 */
        "Canon EOS 50D",
        { 5852, 10000, -578, 10000, -41, 10000, -4691, 10000, 11696, 10000, 3427, 10000, -886, 10000, 2323, 10000, 6879, 10000 },
        { 4920, 10000, 616, 10000, -593, 10000, -6493, 10000, 13964, 10000, 2784, 10000, -1774, 10000, 3178, 10000, 7005, 10000 },
        { 8716, 10000, -692, 10000, 1618, 10000, 3408, 10000, 8077, 10000, -1486, 10000, -13, 10000, -6583, 10000, 14847, 10000 },
        { 9485, 10000, -1150, 10000, 1308, 10000, 4313, 10000, 7807, 10000, -2120, 10000, 293, 10000, -2826, 10000, 10785, 10000 }
    },
    { /* 7 */
        "Canon EOS 550D",
        { 7755, 10000, -2449, 10000, -349, 10000, -3106, 10000, 10222, 10000, 3362, 10000, -156, 10000, 986, 10000, 6409, 10000 },
        { 6941, 10000, -1164, 10000, -857, 10000, -3825, 10000, 11597, 10000, 2534, 10000, -416, 10000, 1540, 10000, 6039, 10000 },
        { 7163, 10000, 1301, 10000, 1179, 10000, 1926, 10000, 9543, 10000, -1469, 10000, -278, 10000, -3830, 10000, 12359, 10000 },
        { 7239, 10000, 1838, 10000, 566, 10000, 2467, 10000, 10246, 10000, -2713, 10000, -112, 10000, -1754, 10000, 10117, 10000 }
        
    },
    { /* 8 */
        "Canon EOS 600D",
        { 7164, 10000, -1916, 10000, -431, 10000, -3361, 10000, 10600, 10000, 3200, 10000, -272, 10000, 1058, 10000, 6442, 10000 },
        { 6461, 10000, -907, 10000, -882, 10000, -4300, 10000, 12184, 10000, 2378, 10000, -819, 10000, 1944, 10000, 5931, 10000 },
        { 7486, 10000, 835, 10000, 1322, 10000, 2099, 10000, 9147, 10000, -1245, 10000, -12, 10000, -3822, 10000, 12085, 10000 },
        { 7359, 10000, 1365, 10000, 918, 10000, 2610, 10000, 9687, 10000, -2297, 10000, 98, 10000, -2155, 10000, 10309, 10000 }
        
    },
    { /* 9 */
        "Canon EOS 650D",
        { 6985, 10000, -1611, 10000, -397, 10000, -3596, 10000, 10749, 10000, 3295, 10000, -349, 10000, 1136, 10000, 6512, 10000 },
        { 6602, 10000, -841, 10000, -939, 10000, -4472, 10000, 12458, 10000, 2247, 10000, -975, 10000, 2039, 10000, 6148, 10000 },
        { 7747, 10000, 485, 10000, 1411, 10000, 2340, 10000, 8840, 10000, -1180, 10000, 105, 10000, -4147, 10000, 12293, 10000 },
        { 7397, 10000, 1199, 10000, 1047, 10000, 2650, 10000, 9355, 10000, -2005, 10000, 193, 10000, -2113, 10000, 10171, 10000 }
        
    },
    { /* 10 */
        "Canon EOS 700D",
        { 6985, 10000, -1611, 10000, -397, 10000, -3596, 10000, 10749, 10000, 3295, 10000, -349, 10000, 1136, 10000, 6512, 10000 },
        { 6602, 10000, -841, 10000, -939, 10000, -4472, 10000, 12458, 10000, 2247, 10000, -975, 10000, 2039, 10000, 6148, 10000 },
        { 7747, 10000, 485, 10000, 1411, 10000, 2340, 10000, 8840, 10000, -1180, 10000, 105, 10000, -4147, 10000, 12293, 10000 },
        { 7397, 10000, 1199, 10000, 1047, 10000, 2650, 10000, 9355, 10000, -2005, 10000, 193, 10000, -2113, 10000, 10171, 10000 }
        
    },
    { /* 11 */
        "Canon EOS 1100D",
        { 6873, 10000, -1696, 10000, -529, 10000, -3659, 10000, 10795, 10000, 3313, 10000, -362, 10000, 1165, 10000, 7234, 10000 },
        { 6444, 10000, -904, 10000, -893, 10000, -4563, 10000, 12308, 10000, 2535, 10000, -903, 10000, 2016, 10000, 6728, 10000 },
        { 7607, 10000, 647, 10000, 1389, 10000, 2337, 10000, 8876, 10000, -1213, 10000, 93, 10000, -3625, 10000, 11783, 10000 },
        { 7357, 10000, 1377, 10000, 909, 10000, 2729, 10000, 9630, 10000, -2359, 10000, 104, 10000, -1940, 10000, 10087, 10000 }
        
    },
    { /* 12 */
        "Canon EOS M",
        { 6985, 10000, -1611, 10000, -397, 10000, -3596, 10000, 10749, 10000, 3295, 10000, -349, 10000, 1136, 10000, 6512, 10000 },
        { 6602, 10000, -841, 10000, -939, 10000, -4472, 10000, 12458, 10000, 2247, 10000, -975, 10000, 2039, 10000, 6148, 10000 },
        { 7747, 10000, 485, 10000, 1411, 10000, 2340, 10000, 8840, 10000, -1180, 10000, 105, 10000, -4147, 10000, 12293, 10000 },
        { 7397, 10000, 1199, 10000, 1047, 10000, 2650, 10000, 9355, 10000, -2005, 10000, 193, 10000, -2113, 10000, 10171, 10000 }
    }
};

/* Matrices from DXO */
double matrix_5d2[9] = {  2.25, -1.50,  0.25,
                         -0.26,  1.62, -0.36,
                         -0.04, -0.40,  1.43  };


static const double xyz_to_rgb[] = {
    3.240710, -0.9692580,  0.0556352,
   -1.537260,  1.8759900, -0.2039960,
   -0.498571,  0.0415557,  1.0570700
};


/* Provides a DXO matrix from camera space to sRGB */
void getMlvCameraTosRGBMatrix(mlvObject_t * video, double * outputMatrix)
{
    double camera_matrix[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
    int32_t * original_matrix;
    uint8_t * camera_name = video->IDNT.cameraName;

    /* Identify camera */
    if (camera_name[10] == '5')
    {
        /* it is either 5D or 50D or 500D or 550D... */
        if (camera_name[11] == 'D')
        {
            /* It is 5D II or III */
            if (camera_name[20] == 'I')
            {
                /* 5D Mark III */
                original_matrix = cam_matrices[0].ColorMatrix2;
            }
            else
            {
                /* 5D Mark II */
                // original_matrix = matrix_5d2;
            }
        }
        else if (camera_name[11] == '0')
        {
            /* It is 50D or 500D */
            if (camera_name[12] == '0')
            {
                /* Is 500D - we use 550d matrix as I don't see 500D */
                original_matrix = cam_matrices[7].ColorMatrix2;
            }
            else
            {
                /* Is 50D */
                original_matrix = cam_matrices[6].ColorMatrix2;
            }
        }
        else
        {
            /* It is 550D! */
            original_matrix = cam_matrices[7].ColorMatrix2;
        }
    }
    else if (camera_name[10] == '6')
    {
        /* It is 6D or 60D or 600D or 650D */
        if (camera_name[11] == 'D')
        {
            /* It is 6D */
            original_matrix = cam_matrices[3].ColorMatrix2;
        }
        else if (camera_name[11] == '0')
        {
            /* Is 60D or 600D */
            if (camera_name[12] == 'D')
            {
                /* It is 60D */
                original_matrix = cam_matrices[5].ColorMatrix2;
            }
            else
            {
                /* It is 600D */
                original_matrix = cam_matrices[8].ColorMatrix2;
            }
        }
        else
        {
            /* It is 650D */
            original_matrix = cam_matrices[9].ColorMatrix2;
        }
    }
    else if (camera_name[10] == '7')
    {
        /* It is 7D or 70D or 700D */
        if (camera_name[11] == 'D')
        {
            /* It is 7D */
            original_matrix = cam_matrices[2].ColorMatrix2;
        }
        else if (camera_name[12] == 'D')
        {
            /* It is 70D */
            original_matrix = cam_matrices[4].ColorMatrix2;
        }
        else
        {
            /* It is 700D; 650D matrix will do right? */
            original_matrix = cam_matrices[9].ColorMatrix2;
        }
    }
    else if (camera_name[10] == '1')
    {
        /* 100D or 1100D (and 1200-16900D) */
        if (camera_name[11] == '1')
        {
            /* It is 1100D */
            original_matrix = cam_matrices[11].ColorMatrix2;
        }
        else if (camera_name[11] == '0')
        {
            /* Is 100D, hopefully 650D matrix will do */
            original_matrix = cam_matrices[9].ColorMatrix2;
        }
        else
        {
            //TODO: Ilia, please fix: in this case original_matrix is uninitialized
        }
    }
    else if (camera_name[10] == 'M')
    {
        /* Is EOS M */
        original_matrix = cam_matrices[12].ColorMatrix2;
    }
    /* As a backup, just do 650D */
    else
    {
        original_matrix = cam_matrices[9].ColorMatrix2;
    }

    /* Convert the silly integere matrix to floaty point */
    matrixRemoveDividers(original_matrix, camera_matrix);

    double out[9];
    invertMatrix(matrix_5d2, out);

    /* giv */
    for (int i = 0; i < 9; ++i)
    {
        outputMatrix[i] = out[i];
    }
}
