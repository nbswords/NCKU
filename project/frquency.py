import matplotlib.pyplot as plt
import pandas as pd

# load ciphertext and processing
f = open('ciphertext1.txt')
ciphertext = f.read()
key = ciphertext.replace('"','').replace('/n','')
char_list = list(key)

key = pd.DataFrame({'chars': char_list})
# drop all the space characters
key =key[key.chars != ' ']
# add a column for aggregation later
key['num'] = 1
# group rows by character type, count the occurences in each group
# and sort by occurance
key = key.groupby('chars').sum() / len(key)
plt.bar(key.index, key.num, width=0.5, color='g')
plt.show()
