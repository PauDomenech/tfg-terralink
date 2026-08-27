# TerraLink

Dispositiu de comunicació senzill per a persones grans, basat en una Raspberry Pi 5, que integra WhatsApp, SMS i (properament) LoRa amb un protocol d'emergència multicanal.

Treball Final de Grau — Grau en Enginyeria Informàtica, Universitat de Girona (2026).

## Funcionalitats principals

- Enviament de missatges de veu transcrits a text, mitjançant faster-whisper (transcripció local, sense connexió externa)
- Recepció i impressió de missatges en paper, via protocol ESC/POS
- Protocol d'emergència: enviament simultani per tots els canals disponibles
- Comunicació per WhatsApp (whatsapp-web.js) i SMS (mòdem 4G Huawei, API HiLink)
- Visualització de l'estat de connexió de cada canal en temps real (icona + indicador lluminós)

## Stack tècnic

- Aplicació principal: Qt6/C++
- Pont de WhatsApp: Node.js (whatsapp-web.js)
- Transcripció de veu: Python (faster-whisper)

## Documentació

- La memòria completa del TFG, amb tots els detalls d'arquitectura, requisits i treball futur, es troba a `/LaTeX` en aquest mateix repositori.
- La documentació tècnica del codi (generada amb Doxygen) està disponible a `/docs`.

## Estat del projecte

Prototip funcional desenvolupat en el marc d'un TFG. No inclou encara sistema de bateria ni integració de LoRa; ambdós es documenten com a treball futur a la memòria.

## Autor

Pau Domènech Villahermosa
Tutor: Dr. Adrià Julià i Juanola — Universitat de Girona
