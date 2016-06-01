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


### Sobre o código

Os tipos de dados básicos e as constantes necessárias estão especificados no arquivo structs.h

HardDiskSimulador é uma classe que encapsula as operações sobre o array de cilindros. Essa classe também
é responsável pela computação do tempo virtual que as operações requerem.

jFat encapsula o sistema de arquivos implementado. A classe gerencia o array de entradas da tabela FAT.
Há também um mapa que liga o nome do arquivo ao seu primeiro setor.

my_drive.cpp possui o main desse trabalho e é responsável pela inicialização dos objetos, a interface com o usuário e a utilização dos métodos fornecidos por HardDiskSimulator e jFat. Por exemplo, jFat apenas fornece
métodos para marcar um setor como ocupado, mas é my_drive.cpp reponsável por requerir e marcar os setores
necessários a escrita do arquivo.


### Utilização

Assim que executado, o programa mostrará um menu com 5 opções. 

Entre com o número da opção desejada, <RETURN> e então siga os passos informados pelo
terminal. Não há verificação de formato da entrada nesse passo. Se você por acaso esquecer de digitar
a opção 1 e entrar diretamente o nome do arquivo, verá diversas mensagens de opção inválida.
Não fazia parte da especificação parsear a entrada.

##### Opção 1 - Escrever arquivo

Entre como o nome de um arquivo que se enconrta no diretório em que o programa está executando.
Entre <RETURN>.
O arquivo será lido para a memória em um passo. O buffer contendo o arquivo será então escrito em
blocos de 512 para o disco virtual. Uma entrada será criada no sistema de arquivos do disco virtual para 
permitir localizar esse arquivo posteriormente.

Se um arquivo com mesmo nome já existir na tabela FAT um erro será mostrado.

##### Opção 2 - Ler arquico

Entre com o nome do arquivo que você deseja recuperar do discu virtual e entre <RETURN>.  Se o arquivo
for localizado na tabela FAT ele será impresso no arquivo SAIDA.txt do diretório em que se executa o programa.

##### Opção 3 - Apagar arquivo

Entre com o nome do arquivo a remover e então <RETURN>
Remove-se a entrada referente ao arquivo informado da tabela FAT e marca os setores previamente ocupados
como livres. Nada é alterado no disco virtual em si.

Se o arquivo não for encontrado na tabela FAT um erro será informado.

##### Opção 4 - Mostra tabela FAT

não requer argumentos adicionais.
Escreve na tela informações dos arquivos contidos na tabela FAT.


##### Opção 5 - Sair

Encessa a execução do programa



### Utilitário para Testes

Para testar o código, escrevi alguns utilitários. Como eles não fazem parte da especificação do trabalho, tomei a liberdade de escrevê-los em python.


A principal tarefa que nosso disco virtual deve realizar é escrever um arquivo e permitir recuperá-lo exatamente como foi escrito.

Algumas ferramentas foram criadas para testar essa funcionalidade.

* generateFile.py

Esse script em python gera um arquivo binário com dados aleatórios. A chamada é:

		python generateFile.py fileBasename QTD_BYTES

O programa ira gerar o aqruivo fileBasenameQTD_BYTES_checkCode.b no diretório atual. Checkcode é um código que pode
ser comparado mais tarde para verificar se não ouve corrupção do arquivo. Você pode obter o mesmo código com o comando
md5sum

A ideia é: gerar um arquivo de dados aleatórios para o qual conhecemos o check sum. Escrevemos esse arquivo no nosso disco virtual e em seguida lemos os dados escritos para o arquivo SAIDA.txt. Rodamos o comando:

	md5sum SAIDA.txt

e comparamos com o código que era esperado.

É interessante testar com arquivos de vários tamanhos para certificar que o disco virtual e o sistema de arquivos implementado
funciona corretamente mesmo com tamanhos que não são múltiplos do tamanho do setor.

