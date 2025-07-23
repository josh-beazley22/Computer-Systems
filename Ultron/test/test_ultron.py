import subprocess

tests = [
    { "cmd": "make test-create", "points": 1 },
    { "cmd": "make test-loading", "points": 1 },
    { "cmd": "make test-adding", "points" : 1 },
    { "cmd": "make test-subtracting", "points" : 1 },
    { "cmd": "make test-branching", "points" : 1 },
    { "cmd": "make test-looping", "points" : 1 },
    { "cmd": "make test-counting-program", "points": 2 },
]

def runtest(t, p):
    completed = subprocess.run(t, shell=True)
    if completed.returncode == 0:
        return p
    else:
        return 0

points = 0

for test in tests:
    points += runtest(test["cmd"], test["points"])

print("Testing count.u...")
match = "= 21\n= 20\n= 19\n= 18\n= 17\n= 16\n= 15\n= 14\n= 13\n= 12\n= 11\n= 10"
command = "echo 22 | ./ultron -f count.u | grep \"{0}\"".format(match)
if runtest(command, 1) == 1:
    print("count.u test passed")
    points += 1
else:
    print("count.u test failed")

print("Check style")
points += runtest("cpplint *.c *.h", 1)

print("Score=" + str(points))
