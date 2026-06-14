# Identificador-de-Imagens
Fiz um identificador de imagens Utilizando um Esp32cam, ele tira foto manda via wifi para um servidor local com uma IA local que reconhece o objeto envia novamente via wifi para o esp32cam e envia o nome do objeto para a tela lcd 16x2 L2C que escreve o nome do objeto, fiz o sistema do áudio sair pelo próprio auto falante do notebook


Alguns avisos:

abrir o serial monitor no baund 115200 se não fica criptografado o texto, no codigo board_config.h na parte Select camera model deixa todos comentados menos o #define CAMERA_MODEL_AI_THINKER // Has PSRA (line 23)

pra fazer as conexões do esp32cam para a tela lcd vc pode soldar os fios que nem eu fiz, soltei umas trilhas em baixo da camera pra usar cabo femea femea pra conectar os dois e passar as informaçõe

https://wokwi.com/projects/361653364762592257 projeto digital com o wokwi.


Sobre o servidor (server.py)
O arquivo server.py é responsável por executar a Inteligência Artificial do projeto. Ele recebe imagens enviadas pela ESP32-CAM através da rede Wi‑Fi, processa essas imagens utilizando o modelo YOLOv8 para reconhecimento de objetos e retorna o nome do objeto identificado para a ESP32.

Além disso, o servidor possui um sistema de voz implementado com pyttsx3, permitindo que o computador fale em voz alta o objeto detectado, auxiliando usuários com deficiência visual. A comunicação entre o ESP32-CAM e o servidor é feita via protocolo HTTP utilizando requisições POST.

Tecnologias utilizadas:


Flask → criação do servidor web local

YOLOv8 → reconhecimento de objetos com IA

Pillow (PIL) → processamento de imagem

pyttsx3 → síntese de voz offline

Threading → execução da voz sem travar o servidor

Funcionamento do fluxo
A ESP32-CAM captura uma imagem.

A imagem é enviada ao servidor Python (server.py).

O modelo YOLOv8 analisa a imagem.

O objeto identificado é traduzido para português.

O computador fala o nome do objeto detectado.

O resultado é enviado de volta para o LCD da ESP32-CAM.
