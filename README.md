# TerraLink

Dispositiu de comunicació senzill per a persones grans, basat en Raspberry Pi 5, que integra WhatsApp, SMS i (properament) LoRa amb un protocol d'emergència multicanal.

Treball Final de Grau — Enginyeria Informàtica, Universitat de Girona (2026).

## Funcionalitats principals
- Enviament de missatges de veu transcrits a text (faster-whisper)
- Recepció i impressió de missatges (ESC/POS)
- Protocol d'emergència: enviament simultani per tots els canals disponibles
- Canals: WhatsApp (whatsapp-web.js), SMS (mòdem 4G Huawei, API HiLink)

## Stack tècnic
- Aplicació principal: Qt6/C++
- Pont de WhatsApp: Node.js (whatsapp-web.js)
- Transcripció de veu: Python (faster-whisper)

## Estat
Prototip funcional desenvolupat com a TFG. Vegeu la memòria completa per a detalls d'arquitectura, requisits i treball futur.

## Autor
Pau Domènech Villahermosa — tutoritzat per Dr. Adrià Julià i Juanola (UdG)
