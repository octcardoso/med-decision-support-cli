#include <iostream>
#include <limits>
#include <cctype>
#include <string>
#include <cmath>

bool is_input_valid(std::string input, 
                    const unsigned short int max_integer_digits, 
                    const unsigned short int max_decimal_digits);

float get_user_input(const unsigned short int max_integer_digits, 
                     const unsigned short int max_decimal_digits);

void get_indicators_amount(unsigned short int *indicators_amount);

void get_indicators(float patients[][7], unsigned short int indicators_amount);

double euclidean_distance(const float patient_indicators_a[], 
                          const float patient_indicators_b[], 
                          const unsigned short int *indicators_amount);

void predict_patient_health(float patients[][7], const unsigned short int *indicators_amount);

int main() {
  
  float patients[11][7];
  unsigned short int indicators_amount = 0;
  
  get_indicators_amount(&indicators_amount);
  get_indicators(patients, indicators_amount);
  predict_patient_health(patients, &indicators_amount);

  return 0;
}

/**
 * @brief Obtém uma entrada numérica do usuário, verifica sua validade, e a converte para float.
 * 
 * Repete a solicitação de entrada até que o usuário insira um valor válido, que deve respeitar:
 * - Um formato numérico válido (ex: "7465.254").
 * - Limites máximos de dígitos nas partes inteira e decimal.
 * 
 * @param max_integer_digits Número máximo de dígitos na parte inteira (antes do ponto decimal).
 * @param max_decimal_digits Número máximo de dígitos na parte decimal (após o ponto decimal).
 * @return float Valor numérico validado convertido para tipo float.
 * @exception std::invalid_argument Se a conversão para float falhar.
 * 
 * @note A validação é feita pela função is_input_valid antes da conversão.
 */
float get_user_input(
  const unsigned short int max_integer_digits, 
  const unsigned short int max_decimal_digits
) {

  std::string user_input;

  getline(std::cin, user_input);
  
  while(!is_input_valid(user_input, max_integer_digits, max_decimal_digits)) { 
    std::cout << "Valor inválido. Insira um valor válido: ";
    getline(std::cin, user_input);
  }

  return std::stof(user_input);
}

/**
 * @brief Valida se uma string representa um número decimal corretamente formatado.
 * 
 * Verifica se:
 * - Formato numérico válido (dígitos, sinal e ponto decimal)
 * - Quantidade máxima de dígitos nas partes inteira e decimal
 * - Eliminação de zeros insignificantes
 * 
 * @param input String contendo a entrada a ser validada
 * @param max_integer_digits Número máximo permitido de dígitos na parte inteira
 * @param max_decimal_digits Número máximo permitido de dígitos na parte decimal
 * @return bool true se a entrada for válida, false caso contrário
 * 
 * @note Comportamentos especiais:
 * - Sinais (+/-) são opcionais e removidos antes da validação
 * - Zeros insignificantes à esquerda na parte inteira são ignorados
 * - Zeros insignificantes à direita na parte decimal são ignorados
 * - Parte decimal de valor zero (ex: ".000") é considerada válida
 */
bool is_input_valid(std::string input, 
  const unsigned short int max_integer_digits, 
  const unsigned short int max_decimal_digits
) {

  if(input.empty()) return false;
  
  if(input[0] == '+' || input[0] == '-') input.erase(0, 1);

  bool has_digit = false;
  int dot_count = 0;

  for(size_t i = 0; i < input.size(); i++) {
    if(input[i] == '.') {
      if(dot_count++ > 1) return false;
    } else if(std::isdigit(input[i])) {
      has_digit = true;
    } else {
      return false;
    }
  }

  if(!has_digit) return false;

  size_t dot_position = input.find('.');
  std::string integer = input.substr(0, dot_position);
  std::string decimal = (dot_position != std::string::npos) ? input.substr(dot_position + 1) : "";

  size_t first_non_zero = integer.find_first_not_of('0');
  if(first_non_zero != std::string::npos) {
    integer = integer.substr(first_non_zero);
  } else {
    integer = "0";
  }

  if(integer.length() > max_integer_digits) return false;

  if(!decimal.empty()) {
    size_t last_non_zero = decimal.find_last_not_of('0');
    if(last_non_zero != std::string::npos) {
      decimal = decimal.substr(0, last_non_zero + 1);
    } else {
      decimal.clear();
    }

    if(decimal.length() > max_decimal_digits) return false;
  }

  return true;
}

/**
 * @brief Solicita e valida a quantidade de indicadores clínicos por paciente
 * 
 * Solicita os indicadores clínicos até que seja fornecido um valor válido entre
 * 3 a 7 indicadores (3 e 7 considerados). Valores fora deste intervalo são 
 * rejeitados com uma mensagem de erro e uma nova solicitação.
 * 
 * @param indicators_amount Ponteiro para armazenar a quantidade validada
 * 
 * @note Características:
 *  - Valores permitidos: 3 a 7 (inclusive)
 *  - Entrada validada em um loop até um valor válido
 */
void get_indicators_amount(unsigned short int *indicators_amount) {
  std::cout << "Insira a quantidade de indicadores usados por paciente: ";

  while(true) {
    *indicators_amount = static_cast<int>(get_user_input(1, 0));

    if (*indicators_amount < 3 || *indicators_amount > 7) {
      std::cout << "A quantidade de indicadores deve ser maior que 2 e menor que 8. Por favor, insira novos indicadores: ";
    } else {
      break;
    }
  }
}

/**
 * @brief Coleta dados clínicos e a classificação de saúde para um vetor de pacientes
 * 
 * A função coleta:
 * - Indicadores clínicos de cada paciente
 * - Classificação de saúde (exceto para o último paciente)
 * 
 * @param patients Vetor para armazenar os dados dos pacientes
 * @param indicators_amount Número total de indicadores por paciente (incluindo a categoria de saúde)
 * 
 * @note Comportamentos especiais:
 *  - Validação da categoria de saúde (+1 ou -1)
 *  - Usa a função get_user_input para validação numérica
 */
void get_indicators(float patients[][7], unsigned short int indicators_amount) {

  const unsigned short int MAX_PATIENTS = 11;
  const unsigned short int HEALTH_CATEGORY_INDEX = indicators_amount - 1;

  for(int patient = 0; patient < MAX_PATIENTS; patient++) {
    std::cout << "\n========== PACIENTE " << patient + 1 << "/" << MAX_PATIENTS << " ==========" << std::endl;
    std::cout << "Insira os " << indicators_amount - 1 << " indicadores:" << std::endl;
    
    for(int indicator = 0; indicator < HEALTH_CATEGORY_INDEX; indicator++) {
      
      std::cout << "[Indicador " << indicator + 1 << "] ";
      std::cout << "Insira um valor: ";

      patients[patient][indicator] = get_user_input(5, 3);
      
    }

    if(patient != MAX_PATIENTS - 1) {
      std::cout << "[Indicador de Saúde] Opções válidas: " << std::endl;
      std::cout << " +1 - Paciente Saudável" << std::endl;
      std::cout << " -1 - Paciente Doente" << std::endl;
      std::cout << "Digite a categoria (+1/-1): ";
  
      while(true) {
        patients[patient][HEALTH_CATEGORY_INDEX] = get_user_input(1, 0);
       
       if(patients[patient][HEALTH_CATEGORY_INDEX] != -1 && patients[patient][HEALTH_CATEGORY_INDEX] != 1) {
         std::cout << "Categoria inválida." << std::endl;
         std::cout << "Use 1 para saudável ou -1 para doente." << std::endl;
         std::cout << "Digite a categoria (1/-1): ";
       } else {
         break;
       }
      }
    } else {
      std::cout << "A saúde desse paciente será definida pelo algoritmo." << std::endl;
    }
    std::cout << "===================================" << std::endl;
  }
}

/**
 * @brief Calcula a distância euclidiana entre dois vetores de indicadores de pacientes
 * 
 * @param patient_indicators_a Vetor de indicadores clínicos
 * @param patient_indicators_b Vetor de indicadores clínicos
 * @param indicators_amount Ponteiro para quantidade de indicadores que serão comparados
 * @return double distância euclidiana entre os dois vetores de indicadores
 */
double euclidean_distance(const float patient_indicators_a[], 
  const float patient_indicators_b[], 
  const unsigned short int *count
) {

  double summation = 0.0;
  double difference = 0.0;

  for(int indicator = 0; indicator < *count; indicator++) {
    difference = patient_indicators_a[indicator] - patient_indicators_b[indicator];
    summation += difference * difference;
  }

  return std::sqrt(summation);
}

/**
 * @brief Determina a classificação de saúde do paciente pela menor distância euclidiana
 * 
 * @param patients Vetor de pacientes com indicadores e classificação de saúde
 * @param indicators_amount Número total de indicadores por paciente (incluindo a categoria de saúde)
 * 
 * @note 
 * - Alerta sobre conflito em caso de empate com classificações diferentes
 */
void predict_patient_health(float patients[][7], const unsigned short int *indicators_amount) {
  const unsigned short int PREDICTION_PATIENT_INDEX = 10;
  const unsigned short int HEALTH_INDEX = *indicators_amount - 1;
  const unsigned short int INDICATORS_COUNT = *indicators_amount - 1;

  double distance = 0.0;
  double shortest_distance = std::numeric_limits<double>::max();
  unsigned short int closest_patient = 0;
  bool conflict = false;

  for(int patient = 0; patient < PREDICTION_PATIENT_INDEX; patient++) {
    distance = euclidean_distance(
      patients[patient],
      patients[PREDICTION_PATIENT_INDEX],
      &INDICATORS_COUNT
    );

    if (distance < shortest_distance) {
      shortest_distance = distance;
      closest_patient = patient;
      conflict = false;
    } else if (distance == shortest_distance) {
      if (patients[patient][HEALTH_INDEX] != patients[closest_patient][HEALTH_INDEX]) {
        conflict = true;
      }
    }
  }
    
  if (conflict) {
    std::cout << "\nAviso: Conflito detectado em distâncias equivalentes!\n";
    std::cout << "Não foi possível determinar uma classificação única.\n";
  } else {
    std::cout << "\nPaciente " << (PREDICTION_PATIENT_INDEX + 1) << " classificado como: "
              << (patients[closest_patient][HEALTH_INDEX] == 1 ? "Saudável" : "Doente")
              << " ( Baseado no paciente " << (closest_patient + 1) << " )\n";
  }
}
