#include <Eigen/Dense>
#include <cmath>

/**
 * Generalized State-Variable Filter structure,
 * as defined by Werner and McClellan (https://dafx2020.mdw.ac.at/proceedings/papers/DAFx2020_paper_70.pdf)
 * 
 * I've added some nonlinear elements of my own design.
 */
class GeneralSVF {
public:
    GeneralSVF();

    void reset();
    void calcCoefs(float r, float k, float wc);
    void setDrive (float newDrive);

    inline float process(float x) {
        Eigen::Matrix<float, 4, 1> v = A_tilde * v_n1 + B_tilde * x;
        float y = (C_tilde * v_n1)(0, 0) + D_tilde(0, 0) * x;
        nonlinearity(v);
        v_n1 = v;
        return y * makeup;
    }

    inline void nonlinearity(Eigen::Matrix<float, 4, 1>& v) {
        v(0,0) = std::tanh(v(0,0) * drive) * invDrive;
        v(2,0) = std::tanh(v(2,0) * drive) * invDrive;
        v(3,0) = std::tanh(v(3,0) * drive) * invDrive;
    }

private:
    Eigen::Matrix<float, 4, 4> A;
    Eigen::Matrix<float, 4, 1> B;
    Eigen::Matrix<float, 1, 4> C;

    Eigen::Matrix<float, 4, 4> A_tilde;
    Eigen::Matrix<float, 4, 1> B_tilde;
    Eigen::Matrix<float, 1, 4> C_tilde;
    Eigen::Matrix<float, 1, 1> D_tilde;

    float g;
    Eigen::Matrix<float, 4, 1> v_n1;

    float drive = 1.0f;
    float invDrive = 1.0f;
    float makeup = 1.0f;
};
