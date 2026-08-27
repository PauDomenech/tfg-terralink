#!/usr/bin/env python3
"""
Script de transcripció d'àudio amb faster-whisper.
Ús: python3 transcriure.py /ruta/al/audio.wav
"""
import sys
import warnings
warnings.filterwarnings("ignore")

from faster_whisper import WhisperModel

def main():
    if len(sys.argv) < 2:
        print("ERROR: Cal passar la ruta de l'àudio", file=sys.stderr)
        sys.exit(1)

    ruta_audio = sys.argv[1]

    try:
        # int8 és el més ràpid per CPU
        model = WhisperModel("small", device="cpu", compute_type="int8")
        segments, info = model.transcribe(ruta_audio, beam_size=5)

        text = " ".join([segment.text for segment in segments]).strip()
        print(text)

    except Exception as e:
        print(f"ERROR: {e}", file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    main()
