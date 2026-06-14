# ==========================
# IMPORTAÇÃO DAS BIBLIOTECAS
# ==========================
# Cria o servidor web Flask
from flask import Flask, request, jsonify
# Biblioteca da IA YOLO
from ultralytics import YOLO
# Manipulação de imagens
from PIL import Image
# Trabalhar com bytes da imagem
import io
# Biblioteca de voz do Windows
import pyttsx3
# Executar tarefas paralelas
import threading


# ==========================
# INICIA O SERVIDOR FLASK
# ==========================
app = Flask(__name__)


# ==========================
# CARREGA A IA YOLO
# ==========================
print("Carregando IA...")

# Modelo de reconhecimento
model = YOLO("yolov8m.pt")


# ==========================
# TRADUÇÃO DOS OBJETOS
# ==========================
# Traduz nomes do inglês
# para português
traducao = {

    "person": "pessoa",
    "cell phone": "celular",
    "bottle": "garrafa",
    "cup": "copo",
    "mouse": "mouse",
    "keyboard": "teclado",
    "book": "livro",
    "chair": "cadeira",
    "tv": "televisao",

    # quando nada é detectado
    "nada": "nenhum objeto"
}


# ==========================
# FUNÇÃO DE VOZ
# ==========================
def falar(texto):
    try:

        # inicia sistema de voz
        engine = pyttsx3.init()

        # velocidade da fala
        engine.setProperty('rate', 150)

        # fala o texto recebido
        engine.say(texto)

        # executa a fala
        engine.runAndWait()

        # encerra o motor de voz
        engine.stop()

    except Exception as e:

        # caso dê erro
        print("Erro voz:", e)


# ==========================
# ROTA PRINCIPAL "/"
# ==========================
# Quando acessar:
# http://IP:5000
@app.route("/")
def home():

    return "Servidor IA online!"


# ==========================
# ROTA DE DETECÇÃO
# ==========================
# Recebe imagem enviada
# pelo ESP32-CAM
@app.route("/detectar", methods=["POST"])
def detectar():

    try:
        print("Requisicao recebida")


        # ==========================
        # VERIFICA SE TEM IMAGEM
        # ==========================
        if not request.data:

            print("Imagem vazia")

            return jsonify({
                "erro": "Imagem vazia"
            }), 400


        # quantidade de bytes
        print(
            "Bytes recebidos:",
            len(request.data)
        )


        # ==========================
        # CONVERTE IMAGEM
        # ==========================
        # transforma bytes
        # em imagem RGB
        imagem = Image.open(
            io.BytesIO(request.data)
        ).convert("RGB")


        # ==========================
        # IA ANALISA A IMAGEM
        # ==========================
        resultados = model(imagem)


        # nomes das classes
        nomes = resultados[0].names

        # caixas detectadas
        caixas = resultados[0].boxes


        # valor padrão
        objeto = "nada"


        # ==========================
        # VERIFICA DETECÇÃO
        # ==========================
        if len(caixas) > 0:

            # pega a primeira classe
            classe = int(
                caixas[0].cls[0]
            )

            # nome do objeto
            objeto = nomes[classe]


        # ==========================
        # TRADUZ OBJETO
        # ==========================
        objeto_pt = traducao.get(
            objeto,
            objeto
        )


        print(
            "Objeto:",
            objeto_pt
        )


        # ==========================
        # FAZ O PC FALAR
        # ==========================
        # cria thread separada
        # para não travar o Flask
        threading.Thread(
            target=falar,
            args=(objeto_pt,)
        ).start()


        # ==========================
        # ENVIA RESPOSTA AO ESP32
        # ==========================
        return jsonify({
            "objeto": objeto_pt
        })


    except Exception as e:

        # ==========================
        # TRATAMENTO DE ERRO
        # ==========================
        print("Erro:", e)

        return jsonify({
            "erro": str(e)
        }), 500


# ==========================
# INICIA O SERVIDOR
# ==========================
if __name__ == "__main__":

    print("Servidor iniciado")

    # libera acesso pela rede
    app.run(
        host="0.0.0.0",
        port=5000
    )
