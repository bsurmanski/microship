typedef struct Matrix{
    float array[2][2];
} Matrix;

typedef struct Vector {
    float x;
    float y;
} Vector;

inline void apply_matrix(Vector *v, Matrix *m);
Matrix *get_rotation_matrix(int degrees);
inline Vector *get_vector();
inline Vector *init_vector(float x, float y);
inline Vector *get_unit_vector(int rotation);
inline void scale_vector(Vector* v, float scale);
inline void set_vector(Vector *v, float x, float y);
