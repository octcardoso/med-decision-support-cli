#include <iostream>
#include <cctype>
#include <iomanip>
#include <string>

bool is_input_valid(std::string input, 
                    const unsigned short int max_integer_digits, 
                    const unsigned short int max_decimal_digits);

float get_user_input(const unsigned short int max_integer_digits, 
                     const unsigned short int max_decimal_digits);

void get_indicators_amount(unsigned short int *indicators_amount);

void get_indicators(float patients[][7], unsigned short int indicators_amount);

void predict_patient_health(float patients[][7], unsigned short int indicators_amount);

int main() {
  
  float patients[11][7];
  unsigned short int indicators_amount = 0;
  
  get_indicators_amount(&indicators_amount);
  get_indicators(patients, indicators_amount);
  predict_patient_health(patients, indicators_amount);

  return 0;
}

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

    std::cout << "===================================" << std::endl;
  }
}

void predict_patient_health(float patients[][7], unsigned short int indicators_amount) {
  // pegar paciente final 
  // calcular distancia euclidiana dele com todos os outros pacientes
  // verificar se nas menores distancias, tem alguma distancia igual e com saude diferente
  // se nao tiver, pegar a menor distancia e determinar a saude do paciente
}
