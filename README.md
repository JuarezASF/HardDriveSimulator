# HardDriveSimulator
Desenvolvido para a matéria de Organização de Arquivos na Universidade de Brasília, 1/2016

Aluno:

    Juarez Aires Sampaio Filho		110032829


Este projeto encontra-se disponível no [github do autor](https://github.com/JuarezASF/HardDriveSimulator)


### Estrutura do Projeto

As instruções a seguir assumem a seguinte estrutura:

	/HardDriveSimulator
		/src
		/py
		/data
		README.md

### Instruções para compilação

#### CMake - Método recomendado 

Se você possui o utilitário cmake instalado, rode o seguinte comando no diretório que contém esse README.

	mkdir build
	cd build
	cmake ..
	make

O projeto deve ter sido compilado dentro de build/src. O arquivo executável se chama my_drive

#### Compilar e Ligar Manualmente

 A partir do diretório com esse readme:

	cd src/
		echo "Compilando"
		g++ --std=c++11 HardDiskSimulator.cpp -c
		g++ --std=c++11 jFat.cpp -c
		g++ --std=c++11 Structs.cpp -c
		g++ --std=c++11 my_drive.cpp -c
		echo "Ligando"
		g++ --std=c++11 *.o -o my_drive         

O arquivo executável se se encontra em src/my_drive



### Utilização

Assim que executado, o programa mostrará um menu com 6 opções. 


### Utilitário para Tests

Para testar o código, escrevi alguns utilitários. Como eles não fazem parte da especificação do trabalho, tomei a liberdade de escrevê-los em python.


A principal tarefa que nosso disco virtual deve realizar é escrever um arquivo e permitir recuperá-lo exatamente como foi escrito.

Algumas ferramentas foram criadas para testar essa funcionalidade.

* generateFile.py

Esse script em python gera um arquivo binário com dados aleatórios. A chamada é:

		python generateFile.py fileBasename QTD_BYTES

O programa ira gerar o aqruiv fileBasenameQTD_BYTES_checkCode.b no diretório atual. Checkcode é um código que pode
ser comparado mais tarde para verificar se não ouve corrupção do arquivo. Você pode obter o mesmo código com o comando
md5sum

A ideia é: gerar um arquivo de dados aleatórios para o qual conhecemos o check sum. Escrevemos esse arquivo no nosso disco virtual e em seguida lemos os dados escritos para o arquivo SAIDA.txt. Rodamos o comando:

	md5sum SAIDA.txt

e comparamos com o código que era esperado.

É interessante testar com arquivos de vários tamanhos para certificar que o disco virtual e o sistema de arquivos implementado
funciona corretamente mesmo com tamanhos que não são múltiplos do tamanho do setor.
