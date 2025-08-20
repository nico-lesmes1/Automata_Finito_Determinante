
with open("conf.txt", encoding="utf-8") as f:
    raw = [l.strip() for l in f if l.strip() and not l.startswith("#")]

estados   = raw[0].split(",")
alfabeto  = raw[1].split(",")
inicial   = raw[-2]
finales   = raw[-1].split(",")

delta = {}
for line in raw[2:-2]:
    parts = line.split(",")
    if len(parts) == 3:
        orig, sym, dest = map(str.strip, parts)
        delta[(orig, sym)] = dest

with open("cadenas.txt", encoding="utf-8") as f:
    cadenas = [l.strip() for l in f if l.strip()]

for cad in cadenas:
    estado = inicial
    for ch in cad:
        estado = delta.get((estado, ch), "ERROR")
        if estado == "ERROR":
            break

    print(f"{cad}: {'acepto' if estado in finales else 'no acepto'}")
