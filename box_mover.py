import random
import time

# Run $python3 box_mover.py | yarp rpc /icubSim/world
# To auto move a blue target

targetColor = "0 255 0"

print("world del all", flush=True)
print(f"world mk sbox 1 1 1 0 3 5 {targetColor}", flush=True)
while (1) :
	time.sleep(3)
	print(f"world set sbox 1 {random.randint(-3, 3)} {random.randint(1, 3)} {random.randint(2, 10)}", flush=True)
