from PIL import Image
a = Image.open("node_true1_xor.png")
#a.show()
green = (166,215,133,255)
red = (224,64,6,255)
violet = (199,191,239,255)
target = violet
pix = a.load()
wid, hei = a.size
for i in range(wid):
    for j in range(hei):
        if pix[i,j] != (255,255,255,0):
            #print pix[i,j]
            pix[i,j] = target
        else:
            pix[i,j] = (255,255,255,0)

a.save("result.png")
