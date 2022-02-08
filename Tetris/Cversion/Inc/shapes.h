#ifndef SHAPES_H
#define SHAPES_H

#define NB_KINDS     7
#define NB_ROTATIONS 4
#define SIZE         4

static int PIECES[NB_KINDS][NB_ROTATIONS][SIZE][SIZE] =
{
    { // O
        {
            {0, 0, 0, 0},
            {0, 1, 2, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0}
        },
 
        {
            {0, 0, 0, 0},
            {0, 1, 2, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0}
        },
 
        {
            {0, 0, 0, 0},
            {0, 1, 2, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0}
        },
 
        {
            {0, 0, 0, 0},
            {0, 1, 2, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0}
        }
    },
 
    { // I
        {
            {0, 0, 0, 0},
            {1, 1, 2, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },
 
        {
            {0, 0, 1, 0},
            {0, 0, 2, 0},
            {0, 0, 1, 0},
            {0, 0, 1, 0}
        },
 
        {
            {0, 0, 0, 0},
            {1, 1, 2, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },
 
        {
            {0, 0, 1, 0},
            {0, 0, 2, 0},
            {0, 0, 1, 0},
            {0, 0, 1, 0}
        }
    },
 
    { // S
        {
            {0, 0, 0, 0},
            {0, 0, 2, 1},
            {0, 1, 1, 0},
            {0, 0, 0, 0}
        },
 
        {
            {0, 0, 1, 0},
            {0, 0, 2, 1},
            {0, 0, 0, 1},
            {0, 0, 0, 0}
        },
 
        {
            {0, 0, 0, 0},
            {0, 0, 2, 1},
            {0, 1, 1, 0},
            {0, 0, 0, 0}
        },
 
        {
            {0, 0, 1, 0},
            {0, 0, 2, 1},
            {0, 0, 0, 1},
            {0, 0, 0, 0}
        }
    },
 
    { // Z
        {
            {0, 0, 0, 0},
            {0, 1, 2, 0},
            {0, 0, 1, 1},
            {0, 0, 0, 0}
        },
 
        {
            {0, 0, 0, 1},
            {0, 0, 2, 1},
            {0, 0, 1, 0},
            {0, 0, 0, 0}
        },
 
        {
            {0, 0, 0, 0},
            {0, 1, 2, 0},
            {0, 0, 1, 1},
            {0, 0, 0, 0}
        },
 
        {
            {0, 0, 0, 1},
            {0, 0, 2, 1},
            {0, 0, 1, 0},
            {0, 0, 0, 0}
        }
    },
 
    { // L
        {
            {0, 0, 0, 0},
            {0, 1, 2, 1},
            {0, 1, 0, 0},
            {0, 0, 0, 0}
        },
 
        {
            {0, 0, 1, 0},
            {0, 0, 2, 0},
            {0, 0, 1, 1},
            {0, 0, 0, 0}
        },
 
        {
            {0, 0, 0, 1},
            {0, 1, 2, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },
 
        {
            {0, 1, 1, 0},
            {0, 0, 2, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 0}
        }
    },
 
    { // J
        {
            {0, 0, 0, 0},
            {0, 1, 2, 1},
            {0, 0, 0, 1},
            {0, 0, 0, 0}
        },
 
        {
            {0, 0, 1, 1},
            {0, 0, 2, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 0}
        },
 
        {
            {0, 1, 0, 0},
            {0, 1, 2, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },
 
        {
            {0, 0, 1, 0},
            {0, 0, 2, 0},
            {0, 1, 1, 0},
            {0, 0, 0, 0}
        }
    },
 
    { // T
        {
            {0, 0, 0, 0},
            {0, 1, 2, 1},
            {0, 0, 1, 0},
            {0, 0, 0, 0}
        },
 
        {
            {0, 0, 1, 0},
            {0, 0, 2, 1},
            {0, 0, 1, 0},
            {0, 0, 0, 0}
        },
 
        {
            {0, 0, 1, 0},
            {0, 1, 2, 1},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },
 
        {
            {0, 0, 1, 0},
            {0, 1, 2, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 0}
        }
    }
};
 
#endif