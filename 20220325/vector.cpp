#include <stdio.h>
#include <math.h>
#include <cmath>

class Vector3d
{
public:
    float x, y, z;
    Vector3d()
    {
        x = 0;
        y = 0;
        z = 0;
    }

    Vector3d(float a, float b, float c)
    {
        x = a;
        y = b;
        z = c;
    }

    Vector3d operator*(float num)
    {
        return Vector3d(x * num, y * num, z * num);
    }
    friend Vector3d operator*(float num, const Vector3d &v)
    {
        return Vector3d(v.x * num, v.y * num, v.z * num);
    }
    Vector3d operator+(const Vector3d &v)
    {
        return Vector3d(x + v.x, y + v.y, z + v.z);
    }
    Vector3d operator-(const Vector3d &v)
    {
        return Vector3d(x - v.x, y - v.y, z - v.z);
    }

    float mag()
    {
        return sqrtf(x * x + y * y + z * z);
    }

    float inner(const Vector3d &v)
    {
        return (x * v.x) + (y * v.y) + (z * v.z);
    }

    Vector3d outer(const Vector3d &v)
    {
        return Vector3d(z * v.y - z * v.y, z * v.x - x * v.z, x * v.y - y * v.z);
    }
};

void printVector(const Vector3d &v)
{
    printf("(%.1f, %.1f, %.1f)\n", v.x, v.y, v.z);
}

int main()
{
    Vector3d a(1, -1, 0), b(2, 3, 6);

    float c = a.mag();
    printf("\n\nMagnitude of Vector 1, 2 : ");
    printf("\nVector 1 = (%f, %f, %f)", c);
    c = b.mag();
    printf("\nVector 2 = (%f, %f, %f)", c);

    printf("\n\nInner product of Vector : ");
    float d = a.inner(b);
    printf("%f", d);
    if (d > 1)
    {
        printf("more than 90degrees\n");
    }
    else if (d < -1)
    {
        printf("less than 90degrees\n");
    }
    else
    {
        printf("90degrees");
    }

    printf("Outer product of Vector : ");
    Vector3d e = a.outer(b);
    printf("%f", e);

    return 0;
}
