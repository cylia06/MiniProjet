#include <iostream>
#include <omp.h>
#include "bitmap_image.hpp"

int FractalDraw()
{
    bitmap_image fractal(2000,2000);
    fractal.clear();

    const unsigned int nombre_d_iterations = 255;

    const double nombre_reel_a = -0.0849;
    const double nombre_imaginaire_b =  0.8721;

    double precedent_nombre_reel, precedent_nombre_imaginaire;

    #pragma omp parallel for private(suivant_nombre_reel,suivant_nombre_imaginaire, precedent_nombre_reel, precedent_nombre_imaginaire )
    for (unsigned int point_i = 0; point_i < fractal.height(); ++point_i)
    {
        for (unsigned int point_j = 0; point_j < fractal.width(); ++point_j)
        {
            double suivant_nombre_reel = 1.5 * (2.0 * point_j / fractal.width () - 1.0);
            double suivant_nombre_imaginaire = (2.0 * point_i / fractal.height() - 1.0);

            for (unsigned int i = 0; i < nombre_d_iterations; ++i)
            {
                precedent_nombre_reel = suivant_nombre_reel;
                precedent_nombre_imaginaire = suivant_nombre_imaginaire;

                suivant_nombre_reel = precedent_nombre_reel * precedent_nombre_reel - precedent_nombre_imaginaire * precedent_nombre_imaginaire + nombre_reel_a;
                suivant_nombre_imaginaire = 2 * precedent_nombre_reel * precedent_nombre_imaginaire + nombre_imaginaire_b;

                if (((suivant_nombre_reel * suivant_nombre_reel) + (suivant_nombre_imaginaire * suivant_nombre_imaginaire)) > 4)
                {
                    unsigned int index = static_cast<int>((1000.0 * i) / nombre_d_iterations);
                    rgb_t c3 = gray_colormap[index];
                    fractal.set_pixel(point_j, point_i, c3.red, c3.green, c3.blue);

                    break;
                }
            }
        }
    }

    fractal.save_image("image.bmp");
    return 0;

}

int main() {
    omp_set_num_threads(4);
    double _begin = omp_get_wtime();
    int res = FractalDraw();
    double _end = omp_get_wtime();
    printf("Elapsed time... %f ms\n", (_end - _begin)*1000);

    return 0;
}