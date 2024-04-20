import random
t = 1000000

ans1 = 0
ans2 = 0
for i in range(t):
    flag = random.choice([0, 1])
    if flag == 0:
        b = [i for i in range(1, 51)]
        x = random.choice(b)
        b.remove(x)
        if 1<= x <= 10:
            ans2 += 1
    else:
        c = [i for i in range(51, 81)]
        x = random.choice(c)
        c.remove(x)
        y = random.choice(c)
        if 51<= x <= 68:
            ans2 += 1


print(ans2/t)

