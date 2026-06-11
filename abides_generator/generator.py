import sys
import os
import zmq
import struct
import pandas as pd
import numpy as np
import time

CURRENT_DIR = os.path.dirname(os.path.abspath(__file__))
ABIDES_PATH = os.path.join(CURRENT_DIR, 'abides_source')
sys.path.append(ABIDES_PATH)

from agent.Agent import Agent
from Kernel import Kernel

class ZmqTraderAgent(Agent):
    def __init__(self, id, name, type, random_state, zmq_port=5555):
        super().__init__(id, name, type, random_state)
        self.zmq_port = zmq_port
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.PUSH)

        self.STRUCT_FORMAT = "<QBBqQ"

        self.order_id = 1
        self.mid_price = 10000

    def kernelStarting(self, startTime):
        super().kernelStarting(startTime)
        print(f"[{self.name}] Łączenie z silnikiem C++ (ZMQ tcp://127.0.0.1:{self.zmq_port})...")
        self.socket.connect(f"tcp://127.0.0.1:{self.zmq_port}")
        self.setWakeup(startTime + pd.Timedelta('1s'))

    def wakeup(self, currentTime):
        super().wakeup(currentTime)

        side = self.random_state.choice([0, 1])
        order_type = 3

        if side == 0:
            price = self.random_state.randint(self.mid_price - 80, self.mid_price)
        else:
            price = self.random_state.randint(self.mid_price, self.mid_price + 80)

        quantity = self.random_state.randint(10, 500)

        binary_order = struct.pack(self.STRUCT_FORMAT, self.order_id, order_type, side, int(price), int(quantity))
        self.socket.send(binary_order)

        self.order_id += 1
        self.mid_price += self.random_state.choice([-2, -1, 0, 1, 2])

        delay = pd.Timedelta(milliseconds=self.random_state.randint(10, 50))
        self.setWakeup(currentTime + delay)

        time.sleep(0.01)


def main():
    print("Inicjalizacja symulacji ABIDES...")

    start_time = pd.to_datetime('2026-06-11 09:30:00')
    end_time = start_time + pd.Timedelta('1 hours')

    seed = 42
    random_state = np.random.RandomState(seed)

    agent = ZmqTraderAgent(
        id=0,
        name="ZmqMarketMaker",
        type="MarketMaker",
        random_state=random_state
    )

    kernel = Kernel(
        kernel_name="LOB_Zmq_Simulation",
        random_state=random_state
    )

    kernel.runner(
        agents=[agent],
        startTime=start_time,
        stopTime=end_time,
        agentLatencyModel=None,
        defaultComputationDelay=0
    )

    print("Symulacja zakończona.")

if __name__ == "__main__":
    main()