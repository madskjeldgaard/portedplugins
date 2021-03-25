#include "GenSVF.hpp"

#define EIGEN_DONT_VECTORIZE
#define EIGEN_UNALIGNED_VECTORIZE

using namespace Eigen;

GeneralSVF::GeneralSVF() {
    A << 0.0f, 1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f, 0.0f,
         0.0f,-1.0f, 0.0f, 1.0f,
         0.0f, 0.0f,-1.0f, 0.0f;

    B << 1.0f, 0.0f, 0.0f, 0.0f;
    C << 0.0f, 0.0f, 0.0f,-1.0f;
}

void GeneralSVF::reset() { v_n1 = Matrix<float, 4, 1>::Zero(); }

void GeneralSVF::setDrive (float newDrive) {
    drive = newDrive;
    invDrive = 1.0f / drive;
    makeup = std::max(1.0f, (float) pow(drive, 0.75f));
}

void GeneralSVF::calcCoefs(float r, float k, float wc) {
    // cook parameters
    g = std::tan(wc);
    A(0, 0) = -2.0f * r;
    A(0, 3) = 4.0f * k * r * r;
    A(2, 2) = A(0, 0);

    // cook discrete state-space matrices
    Matrix<float, 4, 4> I = Matrix<float, 4, 4>::Identity();
    Matrix<float, 4, 4> H = g * (I - g * A).inverse();
    A_tilde = 2 * H * A + I;
    B_tilde = 2 * H * B;
    C_tilde = C * (H * A + I);
    D_tilde = C * B;
}
