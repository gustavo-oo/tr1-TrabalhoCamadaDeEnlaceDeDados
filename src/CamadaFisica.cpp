#include <iostream>
#include <bitset>
#include <vector>
#include <stdint.h>
#include <math.h>

#include "CamadaFisica.hpp"
#include "CamadaEnlace.hpp"

using namespace std;

//Variavel que define a codificação a ser utilizada
uint8_t TIPO_DE_CODIFICACAO;

// ============== Funções dos Protocolos da Camada Física ==============

//Lê a mensagem e a transmite utilizando os três tipos de codificação
void AplicacaoTransmissora(){
    string mensagem;
    cout << "Digite a mensagem a ser enviada: ";
    getline(cin, mensagem);

    TransmitirMensagem(mensagem, CODIFICACAO_BINARIA);
    TransmitirMensagem(mensagem, CODIFICACAO_BIPOLAR);
    TransmitirMensagem(mensagem, CODIFICACAO_MANCHESTER);
}

// Converte a mensagem para bits
void CamadaDeAplicacaoTransmissora(string mensagem){
    vector<int> quadro = StringToBits(mensagem);

    cout << "Mensagem para bits: ";
    PrintVector(quadro);

    CamadaEnlaceDadosTransmissora(quadro);
    //CamadaFisicaTransmissora(quadro);
}

void CamadaFisicaTransmissora(vector<int> quadro){
    vector<int> fluxo_bruto_de_bits;

    switch(TIPO_DE_CODIFICACAO){
        case CODIFICACAO_BINARIA:
            fluxo_bruto_de_bits = CamadaFisicaTransmissoraCodificacaoBinaria(quadro);
            break;
        case CODIFICACAO_MANCHESTER:
            fluxo_bruto_de_bits = CamadaFisicaTransmissoraCodificacaoManchester(quadro);
            break;
        case CODIFICACAO_BIPOLAR:
            fluxo_bruto_de_bits = CamadaFisicaTransmissoraCodificacaoBipolar(quadro);
            break;
    }

    cout << "Bits Codificados: ";
    PrintVector(fluxo_bruto_de_bits);

    MeioDeComunicacao(fluxo_bruto_de_bits);

}

//Simula a transmissão da mensagem do Ponto A ao Ponto B
void MeioDeComunicacao(vector<int> fluxo_bruto_de_bits){
    vector<int> fluxo_bruto_de_bits_ponto_A, fluxo_bruto_de_bits_ponto_B;

    int porcentagemDeErros = 10; //10%, 20%, 30%, 40%, ..., 100%

    fluxo_bruto_de_bits_ponto_A = fluxo_bruto_de_bits;

    cout << "Bit sendo transmitidos..." << endl;
    cout << "Ponto A: ";
    PrintVector(fluxo_bruto_de_bits_ponto_A);

    for(int i = 0; fluxo_bruto_de_bits_ponto_A.size() != fluxo_bruto_de_bits_ponto_B.size(); i++){
        if(rand() % 100 < porcentagemDeErros){
            if(fluxo_bruto_de_bits_ponto_A[i] == VOLTAGEM_POSITIVA){
                fluxo_bruto_de_bits_ponto_B.push_back(fluxo_bruto_de_bits_ponto_A[i] - VOLTAGEM_POSITIVA);

            }else{
                if(fluxo_bruto_de_bits_ponto_A[i] == VOLTAGEM_NEGATIVA){
                    fluxo_bruto_de_bits_ponto_B.push_back(fluxo_bruto_de_bits_ponto_A[i] - VOLTAGEM_NEGATIVA);

                }else{
                    fluxo_bruto_de_bits_ponto_B.push_back(fluxo_bruto_de_bits_ponto_A[i] + VOLTAGEM_POSITIVA);
                }
            }
        }else{
            fluxo_bruto_de_bits_ponto_B.push_back(fluxo_bruto_de_bits_ponto_A[i]);
        }
    }

    cout << "Ponto B: ";
    PrintVector(fluxo_bruto_de_bits_ponto_B);

    CamadaFisicaReceptora(fluxo_bruto_de_bits_ponto_B);
}

// Recebe a mensagem transmitida e a decodifica
void CamadaFisicaReceptora(vector<int> quadro){
    vector<int> fluxo_bruto_de_bits;

    switch(TIPO_DE_CODIFICACAO){
        case CODIFICACAO_BINARIA:
            fluxo_bruto_de_bits = CamadaFisicaReceptoraDecodificacaoBinaria(quadro);
            break;
        case CODIFICACAO_MANCHESTER:
            fluxo_bruto_de_bits = CamadaFisicaReceptoraDecodificacaoManchester(quadro);
            break;
        case CODIFICACAO_BIPOLAR:
            fluxo_bruto_de_bits = CamadaFisicaReceptoraDecodificacaoBipolar(quadro);
            break;
    }

    cout << "Bits decodificados: ";
    PrintVector(fluxo_bruto_de_bits);

    CamadaEnlaceDadosReceptora(fluxo_bruto_de_bits);
    //CamadaDeAplicacaoReceptora(fluxo_bruto_de_bits);
}

//Converte os bits para uma string
void CamadaDeAplicacaoReceptora(vector<int> quadro){
    string mensagem = "";

    mensagem += BitsToString(quadro);
    
    AplicacaoReceptora(mensagem);
}

//Apresenta a mensagem recebida
void AplicacaoReceptora(string mensagem){
    cout << "A mensagem recebida foi: " << mensagem << endl;
}


//// ============== Funções de Codificação ==============
vector<int> CamadaFisicaTransmissoraCodificacaoBinaria(vector<int> quadro){
    vector<int> quadro_codificado = {};

    //Simulando codificação NRZ - do sinal
    for(int i = 0; i < quadro.size(); i++){
        if(quadro[i] == BIT_0){
            quadro_codificado.push_back(VOLTAGEM_ZERO);
        }else if(quadro[i] == BIT_1){
            quadro_codificado.push_back(VOLTAGEM_POSITIVA);
        }
    }

    return quadro_codificado;
}

vector<int> CamadaFisicaTransmissoraCodificacaoManchester(vector<int> quadro){
    vector<int> quadro_codificado = {};
    vector<int> clock = InitClock(quadro.size());

    cout << "Clock:";
    PrintVector(clock);

    int bit_resultante;

    for(int i = 0; i < quadro.size() * 2; i++){
        bit_resultante = quadro[i/2] ^ clock[i];

        if(bit_resultante == BIT_0){
            quadro_codificado.push_back(VOLTAGEM_ZERO);
        }else{
            quadro_codificado.push_back(VOLTAGEM_POSITIVA);
        }        
    }

    return quadro_codificado;
}

// ***************** TO DO *****************
vector<int> CamadaFisicaTransmissoraCodificacaoBipolar(vector<int> quadro){
    vector<int> quadro_codificado = {};

    int ultimo_bit_1 = VOLTAGEM_NEGATIVA;

    for(int i = 0; i < quadro.size(); i++){
        if(quadro[i] == BIT_1){
            if(ultimo_bit_1 == VOLTAGEM_NEGATIVA){
                quadro_codificado.push_back(VOLTAGEM_POSITIVA);
                ultimo_bit_1 = VOLTAGEM_POSITIVA;
            } else {
                quadro_codificado.push_back(VOLTAGEM_NEGATIVA);
                ultimo_bit_1 = VOLTAGEM_NEGATIVA;
            } 
        }else if(quadro[i] == BIT_0){
            quadro_codificado.push_back(VOLTAGEM_ZERO);
        }
    }

    return quadro_codificado;
};


// ============== Funções de Decodificação ==============

vector<int> CamadaFisicaReceptoraDecodificacaoBinaria(vector<int> quadro){
    vector<int> quadro_decodificado = {};

    //Simulando decodificação do sinal
    for(int i = 0; i < quadro.size(); i++){
        if(quadro[i] == VOLTAGEM_ZERO){
            quadro_decodificado.push_back(BIT_0);
        }else{
            quadro_decodificado.push_back(BIT_1);
        }
    }

    return quadro_decodificado;
}


// ***************** TO DO *****************
vector<int> CamadaFisicaReceptoraDecodificacaoManchester(vector<int> quadro){
    vector<int> quadro_decodificado = {};

    for(int i = 0; i < quadro.size(); i++){
        if(i % 2 == 0){
            if(quadro[i] == VOLTAGEM_POSITIVA){
                quadro_decodificado.push_back(BIT_1);

            }else if(quadro[i] == VOLTAGEM_ZERO){
                quadro_decodificado.push_back(BIT_0);
            }
        }
    }

    return quadro_decodificado;
}

// ***************** TO DO *****************
vector<int> CamadaFisicaReceptoraDecodificacaoBipolar(vector<int> quadro){
    vector<int> quadro_decodificado = {};

    for(int i = 0; i < quadro.size(); i++){
        if(quadro[i] == VOLTAGEM_POSITIVA || quadro[i] == VOLTAGEM_NEGATIVA){
            quadro_decodificado.push_back(BIT_1);

        }else if(quadro[i] == VOLTAGEM_ZERO){
            quadro_decodificado.push_back(BIT_0);
        }
    }

    return quadro_decodificado;
}

// ============== Funções Auxiliares ==============

// Converte uma mensagem para bits
vector<int> StringToBits(string mensagem){
    bitset<8> byte;
    vector<int> quadro = {};

    for(int i = 0; i < mensagem.size(); i++){
        byte = bitset<8>(mensagem[i]);

        for(int j = 7; j > -1; j--){
            quadro.push_back(byte[j]);
        }
    }

    return quadro;
}

//converte bits para uma mensagem
string BitsToString(vector<int> quadro){
    uint8_t byte = 0;
    string mensagem;

    for(int i = 0; i < quadro.size(); i++){
        if(i % 8 == 0){
            mensagem += (char) byte;
            byte = 0;
        }


        byte += quadro[i] * pow(2, 7 - (i%8));
    }

    mensagem += char (byte) + '\0';

    return mensagem;
}

//Faz o print do vetor
void PrintVector(vector<int> vetor){
    for(int i = 0; i < vetor.size(); i++){
        cout << vetor[i] << ' ';
    }
    cout << '\n' << endl;
}

// Altera a codificação a ser utilizada e passa a mensagem para a camada de Transmissão
void TransmitirMensagem(string mensagem, uint8_t codificacao){
    TIPO_DE_CODIFICACAO = codificacao;

    switch(TIPO_DE_CODIFICACAO){
        case CODIFICACAO_BINARIA:
            cout << "\n=========== CODIFICAÇÃO BINÁRIA ===========" << endl;
            break;

        case CODIFICACAO_MANCHESTER:
            cout << "\n=========== CODIFICAÇÃO MANCHESTER ===========" << endl;
            break;

        case CODIFICACAO_BIPOLAR:
            cout << "\n=========== CODIFICAÇÃO BIPOLAR ===========" << endl;
            break;
    }

    CamadaDeAplicacaoTransmissora(mensagem);
}

vector<int> InitClock(int tamanho){
    vector<int> clock = {};

    for(int i = 0; i < tamanho; i++){
        clock.push_back(BIT_0);
        clock.push_back(BIT_1);
    }

    return clock;
}

