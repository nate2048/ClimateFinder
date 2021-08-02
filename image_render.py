# Library and module import statements.
from PIL import Image
import csv

# Declare Image objects for image output and input.
world = Image.open('images/world.png')
circle = Image.open('images/circle.png')
star = Image.open('images/star.png')

latitudes = []
longitudes = []

# Read latitude and longitude coordinates from CSV file.
with open('locations.csv') as file:
    openFile = csv.reader(file)
    for row in openFile:
        latitudes.append((int(-1*(float(row[0]) - 90) * 7)))
        longitudes.append((int((float(row[1]) + 180) * 7)))

# Create an annotation for every coordinate in the CSV file.
for index in range(len(latitudes)):
    if index == 0:
        world.paste(star, ((longitudes[index] - int(circle.width/2)), (latitudes[index] - int(circle.height/2))), star)
    else:
        world.paste(circle, ((longitudes[index] - int(circle.width/2)), (latitudes[index] - int(circle.height/2))), circle)

# Save new image.
world.save('ideal_locations.png')