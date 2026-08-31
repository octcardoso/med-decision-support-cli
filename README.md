# Med Decision Support CLI

Ferramenta de linha de comando em C++ para apoio à decisão médica, que classifica a condição de saúde de um paciente com base na similaridade com um conjunto de pacientes já classificados.

---

## Como funciona

O programa recebe um conjunto de pacientes com indicadores clínicos numéricos e uma classificação de saúde conhecida (saudável ou doente). Um último paciente, sem classificação, é comparado a todos os demais usando **distância euclidiana** entre os vetores de indicadores.

A classificação prevista é a do paciente mais próximo (menor distância). Em caso de empate entre pacientes com classificações diferentes, o programa alerta sobre o conflito em vez de arriscar uma resposta incorreta.

---

## Funcionalidades

- Validação robusta de entrada numérica (limite de dígitos inteiros e decimais, sinais opcionais, remoção de zeros insignificantes)
- Quantidade de indicadores clínicos configurável (de 3 a 7 por paciente)
- Cálculo de distância euclidiana entre vetores de indicadores
- Detecção de empates/conflitos na classificação final

---

## Como compilar e rodar

**Pré-requisitos:** um compilador C++ compatível com C++11 ou superior (g++, clang++, etc).

```bash
g++ -o med-decision-support med-decision-support-cli.cpp
./med-decision-support
```

No Windows (com g++ via MinGW):
```bash
g++ -o med-decision-support.exe med-decision-support-cli.cpp
med-decision-support.exe
```

---

## Uso

1. Informe a quantidade de indicadores clínicos por paciente (entre 3 e 7).
2. Para cada um dos 10 primeiros pacientes, insira os valores dos indicadores e a classificação de saúde conhecida (`+1` saudável / `-1` doente).
3. Para o 11º paciente, insira apenas os indicadores — a classificação será prevista pelo programa.

---

## Limitações

- Número fixo de pacientes (11, sendo o último o caso a ser previsto).
- Sem persistência de dados — cada execução parte do zero.
- Todos os indicadores têm o mesmo peso no cálculo de distância (sem normalização ou ponderação).

---

## Nota

Este projeto foi originalmente desenvolvido em 2025 e migrado para este repositório em 2026. O histórico de commits foi preservado.