#include "CamadaEnlace.hpp"
#include <iostream>
// #include <bitset>
// #include <vector>
// #include <stdint.h>
// #include <math.h>

vector<int> CamadaEnlaceDadosTransmissoraEnquadramentoInsercaoDeBytes(vector<int> quadro) {
    vector <int> quadroEnquadrado;
    quadroEnquadrado.push_back(BYTE_DE_FLAG);

    for (int i = 0; i < quadro.size(); i++){
        if (quadro[i] == BYTE_DE_FLAG || quadro[i] == ESC)
            quadroEnquadrado.push_back(ESC);
        quadroEnquadrado.push_back(quadro[i]);
    }
    quadroEnquadrado.push_back(BYTE_DE_FLAG);

    return quadroEnquadrado;
}

vector<int> CamadaEnlaceDadosReceptoraEnquadramentoInsercaoDeBytes(vector<int> quadro) {
    vector <int> quadroDesenquadrado;

    // Loop sem os elementos das pontas do quadro, por serem sempre o byte de flag
    for (int i = 1; i < quadro.size() - 1; i++){
        if (quadro[i] != ESC)
            quadroDesenquadrado.push_back(quadro[i]);
        else if (quadro[i - 1] == ESC)
            quadroDesenquadrado.push_back(quadro[i]);
    }

    return quadroDesenquadrado;
}

vector<int> CamadaEnlaceDadosReceptoraEnquadramento (vector<int> quadro){
    int tipoDeEnquadramento = 1; // Alterar de acordo com o teste
    vector<int> quadroDesenquadrado;

    switch (tipoDeEnquadramento){
    case 0:
        // quadroDesenquadrado = CamadaEnlaceDadosReceptoraEnquadramentoContagemDeCaracteres(quadro);
        break;
    
    case 1:
        quadroDesenquadrado = CamadaEnlaceDadosReceptoraEnquadramentoInsercaoDeBytes(quadro);
        break;
    }
}

vector<int> CamadaEnlaceDadosReceptora (vector<int> quadro){
    CamadaEnlaceDadosReceptoraEnquadramento(quadro);

    // CamadaEnlaceDadosReceptoraControleDeErro(quadro);

    // CamadaDeAplicacaoReceptora(quadro);
}
