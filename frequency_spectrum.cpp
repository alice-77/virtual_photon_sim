// ***********************************************************************
// Programa de cálculo de número de fótons equivalentes usando a biblioteca
// de cálculo numérico científico GNU Scientific Library (GSL)
// ***********************************************************************
//
// O programa faz os cálculos em SI e, após isso, salva o resultado no arquivo
// em unidades naturais. Em nota, achei mais fácil assim.

#include<iostream>
#include<fstream>
#include<cassert>
#include<cmath>

//Inclusão da bilbioteca das funções para partícula pontual.
#include"headers/point_like_charge.hpp"


// Faz o output dos pontos de N1 e N2 em um mapa bidimensional (x,y), levando
// em conta o requisito do GNUPLOT de colocar uma linha em branco para cada
// atualização do valor de x.
void output_N1_and_N2(const char FNAME_N1[50], const char FNAME_N2[50])
{
	const int NUM_PONTOS = 100;

	const double LOWER_B = 0.5e-16;
	const double UPPER_B = 50e-15;

	const double LOWER_F = 0.1e+9;
	const double UPPER_F = 6.0e+9;

	double b = LOWER_B;
	double freq = LOWER_F;
	double step_par = (UPPER_B - LOWER_B)/NUM_PONTOS;
	double step_freq = (UPPER_F - LOWER_F)/NUM_PONTOS;

	std::ofstream dados_out1(FNAME_N1);
	std::ofstream dados_out2(FNAME_N2);

	assert(dados_out1.is_open());
	assert(dados_out2.is_open());

	std::cout << "\n * Parâmetros dos mapas bidimensionais de N1 e N2 \n";
	std::cout << "\tIntervalo de parâmetro de impacto:\t[" << LOWER_B << ":" << UPPER_B << "] \n"; 
	std::cout << "\tIntervalo de frequência:\t\t[" << LOWER_F << ":" << UPPER_F << "] \n";
	std::cout << "\tNúmero de pontos = " << NUM_PONTOS*NUM_PONTOS << "\n";
	std::cout << "\tDados salvos nos arquivos \'" << FNAME_N1 << "\' e \'" << FNAME_N2 << "\'\n";

	for(int i = 0; i <= NUM_PONTOS; i++){
		b = LOWER_B;

		for(int k = 0; k <= NUM_PONTOS; k++){
			dados_out1 << freq << "\t" << b << "\t" << ep_num_par(freq,b) << "\n";
			dados_out2 << freq << "\t" << b << "\t" << ep_num_perp(freq,b) << "\n";

			b += step_par;
		}

		dados_out1 << "\n";
		dados_out2 << "\n";
		freq += step_freq;
	}

	dados_out1.close();
	dados_out2.close();
}

// Printa a razão entre o N1 e o N2 para uma escolha da frequencia
void output_ratio(const char FNAME[50])
{
	const int NUM_PONTOS = 1000;
	const double LOWER_B = 0.5e-16;
	const double UPPER_B = 50.0e-14;
	const double STEP = (UPPER_B - LOWER_B) / NUM_PONTOS;
	const double FREQ = 10.0e+9;

	std::ofstream dados_out(FNAME);
	assert(dados_out.is_open());

	double b = LOWER_B;
	double ratio;

	std::cout << "\n * Parâmetros da curva N1/N2 \n";
	std::cout << "\tIntervalo de parâmetro de impacto:\t[" << LOWER_B << ":" << UPPER_B << "]\n";
	std::cout << "\tFrequência utilizada = " << FREQ << "\n";
	std::cout << "\tNúmero de pontos = " << NUM_PONTOS << "\n";

	for(int i = 0; i <= NUM_PONTOS; i++){
		ratio = ep_num_par(FREQ,b) / ep_num_perp(FREQ,b);
		dados_out << b << "\t" << ratio << "\n";
		b += STEP;
	}
	
	std::cout << "\tDados salvos no arquivo \'" << FNAME << "\'\n";

	dados_out.close();
}

// Faz o output do número de fótons totais integrados sobre o parâmetro de
// impacto
void output_n_total(const char FNAME[50])
{
	const int NUM_PONTOS = 5000;

	// Intervalos de frequencia em eletron-Volt
	const double LOWER_F = 1.0e+9;
	const double UPPER_F = 50.0e+9; 

	double freq = LOWER_F;
	double step = (UPPER_F - LOWER_F)/NUM_PONTOS;

	std::ofstream dados_out(FNAME);
	assert(dados_out.is_open());
	
	std::cout << "\n * Parâmetros da curva n \n";
	std::cout << "\tIntervalo de frequência:\t[" << LOWER_F << ":" << UPPER_F << "]\n";
	std::cout << "\tParâmetro de impacto mínimo = " << GSL_CONST_MKSA_BOHR_RADIUS << "\n";
	std::cout << "\tNúmero de pontos = " << NUM_PONTOS << "\n";
	std::cout << "\tDados salvos no arquivo \'" << FNAME << "\'\n";

	for(int i = 0; i <= NUM_PONTOS; i++){
		dados_out << freq << "\t" << ep_num_total(freq) << "\n";
		freq += step;
	}

	dados_out.close();
}


int main(int argc, char *argv[])
{
	std::cout << "-----------------------------------\n";
	std::cout << "Iniciando cálculo dos pontos.\n";

	output_N1_and_N2("data/N1_map.dat","data/N2_map.dat");
	output_ratio("data/ratio.dat");
	output_n_total("data/n_total.dat");

	std::cout << "\nCálculo concluído\n\n";

	return 0;
}
