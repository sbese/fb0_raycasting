class test():
    def __init__(self):
        self.a = 0

    def inc(self):
        self.a += 1


class test2():
    def __init__(self):
        pass

    def f(self, arg):
        self.arg = arg
        self.arg.inc()


t = test()
print(t.a)
t.inc()
print(t.a)
t2 = test2()
t2.f(t)
print(t.a)

# 0
# 1
# 2
