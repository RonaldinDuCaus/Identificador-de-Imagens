# Identificador-de-Imagens
Fiz um identificador de imagens Utilizando um Esp32cam, ele tira foto manda via wifi para um servidor local com uma IA local que reconhece o objeto envia novamente via wifi para o esp32cam e envia o nome do objeto para a tela lcd 16x2 L2C que escreve o nome do objeto, fiz o sistema do áudio sair pelo próprio auto falante do notebook


Alguns avisos:

abrir o serial monitor no baund 115200 se não fica criptografado o texto, no codigo board_config.h na parte Select camera model deixa todos comentados menos o #define CAMERA_MODEL_AI_THINKER // Has PSRA (line 23)

pra fazer as conexões do esp32cam para a tela lcd vc pode soldar os fios que nem eu fiz, soltei umas trilhas em baixo da camera pra usar cabo femea femea pra conectar os dois e passar as informaçõe

https://wokwi.com/projects/361653364762592257 projeto digital com o wokwi.
