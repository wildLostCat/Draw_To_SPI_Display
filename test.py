import json

positions = set()
positions.add((19, 50))
positions.add((129, 3350))
pos_list = list(positions)

js = json.dumps(pos_list)

print(js)
