#include "KMacierzA.h"
#include <iostream>


KMacierzA::KMacierzA() {
    w = 0;
    k = 0;
    M.empty();
}

KMacierzA::KMacierzA(unsigned iw){
    w = iw;
    k = iw;
    M.resize(w);
    for (unsigned i{ 0 }; i < M.size(); ++i)
        M[i].resize(k, 0);
}

KMacierzA::KMacierzA(unsigned iw, unsigned ik){
    w = iw;
    k = ik;
    M.resize(w);
    for (unsigned i{ 0 }; i < M.size(); ++i)
        M[i].resize(k, 0);
}

KMacierzA::~KMacierzA() {

}

KMacierzA::KMacierzA(const KMacierzA& iM) {
    w = iM.getWiersze();
    k = iM.getKolumny();
    M.resize(w);
    for (unsigned i{ 0 }; i < w; ++i) {
        M[i].resize(k, 0);
        for (unsigned j{ 0 }; j < k; ++j)
            M[i][j] = iM.M[i][j];
    }
}

//
// Przeciazenia operatorow
//
KMacierzA KMacierzA::operator+(KMacierzA& iM) {
    KMacierzA sum(w, k);
     for (unsigned i{ 0 }; i < w; ++i)
        for (unsigned j{ 0 }; j < k; ++j)
            sum(i, j) = this->M[i][j] + iM(i, j);
    return sum;
}

KMacierzA KMacierzA::operator-(KMacierzA& iM) {
    KMacierzA dif(w, k);
    for (unsigned i{ 0 }; i < w; ++i)
        for (unsigned j{ 0 }; j < k; ++j)
            dif(i, j) = this->M[i][j] - iM(i, j);
    return dif;
}

KMacierzA KMacierzA::operator*(KMacierzA& iM) {
    KMacierzA pro(w, iM.getKolumny());
    if (k == iM.getWiersze()){
        float buff = 0.0;
        for (unsigned i{ 0 }; i < w; ++i){
            for (unsigned j{ 0 }; j < iM.getKolumny(); ++j){
                buff = 0.0;
                for (unsigned l{ 0 }; l < k; ++l)
                    buff += M[i][l] * iM(l, j);
                pro(i, j) = buff;
                //cout << pro(i,j) << " ";
            }
            //cout << endl;
        }
    }
    //else
    //    std::cout<<"\n\nBlad!\n\n";
    return pro;   
}

/*void KMacierzA::operator=(KMacierzA& iM) {
    w = iM.getWiersze();
    k = iM.getKolumny();
    M.empty();
    M.resize(w);
    for (unsigned i{ 0 }; i < M.size(); ++i) {
        M[i].resize(k, 0);
        for (unsigned j{ 0 }; j < k; ++j)
            this->M[i][j] = iM(i, j);
    }
}*/

bool KMacierzA::operator!=(float iskalar) {
    bool bul = false;
    for (unsigned i{ 0 }; i < M.size(); ++i) {
        for (unsigned j{ 0 }; j < k; ++j)
            if (this->M[i][j] - iskalar == 0) {
                return true;
            }
    }
    return false;
}

KMacierzA KMacierzA::operator+(float iskalar) {
    KMacierzA sum(w, k);
    for (unsigned i{ 0 }; i < w; ++i)
        for (unsigned j{ 0 }; j < k; ++j)
            sum(i, j) = this->M[i][j] + iskalar;
    return sum;
}

KMacierzA KMacierzA::operator-(float iskalar) {
    KMacierzA dif(w, k);
    for (unsigned i{ 0 }; i < w; ++i)
        for (unsigned j{ 0 }; j < k; ++j)
            dif(i, j) = this->M[i][j] - iskalar;
    return dif;
}

KMacierzA KMacierzA::operator*(float iskalar) {
    KMacierzA pro(w, k);
    for (unsigned i{ 0 }; i < w; ++i)
        for (unsigned j{ 0 }; j < k; ++j)
            pro(i, j) = this->M[i][j] *iskalar;
    return pro;
}

KMacierzA KMacierzA::operator/(float iskalar) {
    KMacierzA quo(w, k);
    for (unsigned i{ 0 }; i < w; ++i)
        for (unsigned j{ 0 }; j < k; ++j)
            quo(i, j) = this->M[i][j] /iskalar;
    return quo;
}

// Returns value of given location when asked in the form A(x,y)
float& KMacierzA::operator()(const unsigned& rowNo, const unsigned& colNo){
    return this->M[rowNo][colNo];
}

// No brainer - returns row #
unsigned KMacierzA::getWiersze() const{
    return this->w;
}

// returns col #
unsigned KMacierzA::getKolumny() const{
    return this->k;
}

// Prints the matrix beautifully
void KMacierzA::Drukuj() const{
    std::cout << "Macierz wspó³czynników czy czegotam " << std::endl;
    for (unsigned i{ 0 }; i < w; ++i) {
        for (unsigned j{ 0 }; j < k; ++j)
            std::cout << "[" << M[i][j] << "] ";
        std::cout << std::endl;
    }
}

// Take any given matrices transpose and returns another matrix
KMacierzA KMacierzA::T() {
    KMacierzA trans(k, w);
    for (unsigned i{ 0 }; i < k; ++i)
        for (unsigned j{ 0 }; j < w; ++j)
            trans(i, j) = this->M[j][i];
    return trans;
}


//credits: https://stackoverflow.com/questions/60300482/c-calculating-the-inverse-of-a-matrix

KMacierzA KMacierzA::Inverse() {
    KMacierzA inverted(w, k);

    if(getDeterminant(M) == 0)
        throw std::runtime_error("Determinant is 0");

    double d = 1.0/getDeterminant(M);
    std::vector<std::vector<float>> invertigation(M.size(), std::vector<float> (M.size()));

    for (size_t i{ 0 }; i < M.size(); ++i)
        for (size_t j{ 0 }; j < M.size(); ++j)
            invertigation[i][j] = M[i][j] * d;

    //return getTranspose(getCofactor(solution));
    invertigation = getCofactor(invertigation);

    for (size_t i{ 0 }; i < invertigation.size(); ++i)
        for (size_t j{ 0 }; j < invertigation.size(); ++j)
            inverted(i, j) = invertigation[i][j];

    return inverted.T();
}

float getDeterminant(const std::vector<std::vector<float>> vect) {
    if (vect.size() != vect[0].size())
        throw std::runtime_error("Matrix is not quadratic");

    int dimension = vect.size();
    if (dimension == 0) return 1;
    if (dimension == 1) return vect[0][0];
    if (dimension == 2) return vect[0][0] * vect[1][1] - vect[0][1] * vect[1][0];

    double result = 0;
    int sign = 1;
    for (int i{ 0 }; i < dimension; ++i) {
        //Submatrix
        std::vector<std::vector<float>> subVect(dimension - 1, std::vector<float>(dimension - 1));
        for (int m = 1; m < dimension; m++) {
            int z = 0;
            for (int n = 0; n < dimension; n++) {
                if (n != i) {
                    subVect[m - 1][z] = vect[m][n];
                    z++;
                }
            }
        }
        //recursive call
        result = result + sign * vect[0][i] * getDeterminant(subVect);
        sign = -sign;
    }

    return result;
}

std::vector<std::vector<float>> getCofactor(const std::vector<std::vector<float>> vect) {
    if (vect.size() != vect[0].size())
        throw std::runtime_error("Matrix is not quadratic");

    std::vector<std::vector<float>> solution(vect.size(), std::vector<float>(vect.size()));
    std::vector<std::vector<float>> subVect(vect.size() - 1, std::vector<float>(vect.size() - 1));

    for (std::size_t i = 0; i < vect.size(); i++) {
        for (std::size_t j = 0; j < vect[0].size(); j++) {
            int p = 0;
            for (size_t x = 0; x < vect.size(); x++) {
                if (x == i) continue;
                int q = 0;

                for (size_t y = 0; y < vect.size(); y++) {
                    if (y == j) continue;
                    subVect[p][q] = vect[x][y];
                    q++;
                }
                p++;
            }
            solution[i][j] = pow(-1, i + j) * getDeterminant(subVect);
        }
    }
    return solution;
}