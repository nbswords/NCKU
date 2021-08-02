seats = ["1A", "1B", "1C", "1D", "2A", "2B", "2C", "2D",
         "3A", "3B", "3C", "3D", "4A", "4B", "4C", "4D",
         "5A", "5B", "5C", "5D", "6A", "6B", "6C", "6D",
         "7A", "7B", "7C", "7D"]

def check_is_full(seats):
  count = 0
  for i in range(0, 27):
     if seats[i] == 'X':
        count += 1
  if count == 28:
      return 1
  else:
      return 0

Order = []
judge = 0
while judge == 0:
  '''印出現有座位'''
  print("Here are seats numbers:")
  for i in range(0, 7):
    print(i+1, end=" ")
    for j in range(0, 4):
      print(seats[i*4+j], end=" ")
    print(end='\n')

  '''詢問要訂哪些位置'''
  Order.append(input("Please input the seats you want ONCE Again EX:1A: "))
  for k in range(len(Order)):
    for j in range(0, 28):
          if Order[k] == seats[j]:
              seats[j] = 'X'
  '''檢查座位是否滿了'''
  judge = check_is_full(seats)
print("All of seats are full !!!")
