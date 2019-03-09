#coding = Big5

Zh2Ch = {}



with open('Big5-ZhuYin.map','r',encoding = 'big5hkscs') as fp:
    for line in fp:
        #line = line.decode('big5')
        #print(line)
        title = line.split(' ')
        Zh2Ch[title[0]] = title[0]
        #print(title)
        div = title[1].split('/')
        #print(div)
        for step in div:
            if step[:1] not in Zh2Ch:
                Zh2Ch[step[:1]] = title[0]
            else:
                if title[0] in Zh2Ch[step[:1]]:
                    break
                else:
                    temp = Zh2Ch[step[:1]]
                    temp = temp + ' ' + title[0]
                    Zh2Ch[step[:1]] = temp
                    


#print(len(Zh2Ch))
#print('-----')
fp.close()
with open('ZhuYin-Big5.map', 'w' , encoding = 'big5hkscs') as fp:
    for key in Zh2Ch:
        print(str(key) + '\t' + str(Zh2Ch[key]), file = fp)
fp.close()
