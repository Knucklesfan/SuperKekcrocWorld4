from string import ascii_lowercase
from string import ascii_uppercase

numbers = {07,07,07,07,07,07,07,07,07,07,07,07,07,07,06,06,07,07,05,07,08,06,09,08,07,07,07,07,06,07,07,07,11,07,07,07,07,07,06,07,07,06,07,07,03,06,07,03,11,07,07,07,07,06,06,05,07,07,11,07,07,07,07,07,07,07,10,06,07,10,06,06,06,06,05,05,05,05,08,07,07,07,07,06,07,07,07,07,07,07,07,07,07,11,06,11,07,07,07,07,04,04,05,05,07,07,07,07,07,07,07,07,07,07,07,03,03,07,07,07,07,06,06,06,06,04,04,03}
def makeword(word, posx, posy):
    print(""""<letter>\n 
        <posx>""" + str(posx) + """</posx>\n 
        <posy>""" + str(posy) + """<</posy>\n
        <char>""" + str(word) + """<</char>\n
    </letter>\n""")


rows = 8
maxnumber = 0
posx = 0
posy = 0
print("<letters>\n")
while maxnumber < 10:
    makeword(maxnumber, posx, posy)
    maxnumber += 1
    posx += 1
    posy = int( posx / rows)
for word in ascii_uppercase:
    makeword(word, posx, posy)
    posx += 1
    posy = int( posx / rows)
for word in ascii_lowercase:
    makeword(word, posx, posy)
    posx += 1
    posy = int( posx / rows)
