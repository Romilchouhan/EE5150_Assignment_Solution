########## Instructions to run the code ##########
# python3 plotting.py --datapath <path to data> --savepath <path to save plots> --plot_p <True/False> --plot_delay <True/False> --plot_packet_drops <True/False> --plot_TCP_tp <True/False> --plot_UDP_tp <True/False> --plot_TCP_tp_three <True/False>

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--datapath', type=str, default=None, help='path to data')
parser.add_argument('--savepath', type=str, default='plots/', help='path to save plots')
parser.add_argument('--plot_p', type=str, default=False, help='plot with varying p')
parser.add_argument('--plot_delay', type=str, default=False, help='plot delay')
parser.add_argument('--plot_packet_drops', type=str, default=False, help='plot packet drops')
parser.add_argument('--plot_TCP_tp', type=str, default=False, help='plot TCP throughput')
parser.add_argument('--plot_UDP_tp', type=str, default=False, help='plot UDP throughput')
parser.add_argument('--plot_TCP_tp_three', type=str, default=False, help='plot TCP throughput for all three algorithms')
args = parser.parse_args()


def plot_with_varying_p():
    # Plot UDP_tp, TCP_tp, server_utility against p
    df = pd.read_csv(args.datapath, sep='\t', index_col=None)
    df = pd.DataFrame(df)
    plt.figure(figsize=(4,4))
    plt.grid(True)
    plt.plot(df['p'], df['UDP_throughput'], label='UDP_throughput')
    plt.plot(df['p'], df['TCP_throughput'], label='TCP_throughput')
    plt.plot(df['p'], df['Server_Utilisation'], label='server_utility')
    plt.title("UDP_throughput, TCP_throughput, server_utility")
    plt.xlabel("arrival rate")
    plt.legend()
    plt.savefig(args.savepath+"UDP_throughput, TCP_throughput, server_utility.png")
    plt.show()

def plot_delay():
    # Plot UDP_delay and TCP_delay against p
    df = pd.read_csv(args.datapath, sep='\t', index_col=None)
    df = pd.DataFrame(df)
    plt.figure(figsize=(6,4))
    plt.grid(True)
    plt.plot(df['p'], df['UDP_delay'], label='UDP_delay')
    plt.plot(df['p'], df['TCP_delay'], label='TCP_delay')
    plt.title("UDP_delay, TCP_delay for N=60")
    plt.xlabel("arrival rate")  
    plt.legend()
    plt.savefig(args.savepath+"UDP_delay, TCP_delay.png")
    plt.show()

def plot_packet_drops():
    # Plot the packet drops against p
    df = pd.read_csv(args.datapath, sep='\t', index_col=None)
    df = pd.DataFrame(df)
    plt.figure(figsize=(6,4))
    plt.grid(True)
    plt.plot(df['p'], df['UDP_packet_drop'], label='UDP_packet_drops')
    plt.plot(df['p'], df['TCP_packet_drop'], label='TCP_packet_drops')
    plt.title("UDP_packet_drops, TCP_packet_drops for N=30")
    plt.xlabel("arrival rate")
    plt.legend()
    plt.savefig(args.savepath+"UDP_packet_drops, TCP_packet_drops.png")
    plt.show()

def plot_TCP_tp():
    # Plot TCP_throughput vs p
    df = pd.read_csv(args.datapath, sep='\t', index_col=None)
    df = pd.DataFrame(df)
    plt.figure(figsize=(6,4))
    plt.grid(True)
    plt.plot(df['p'], df['TCP_throughput'], label='TCP_throughput')
    plt.title("TCP_throughput for N=60")
    plt.xlabel("arrival rate")
    plt.legend()
    plt.savefig(args.savepath+"TCP_throughput.png")
    plt.show()

def plot_UDP_tp():
    # Plot UDP_throughput vs p
    df = pd.read_csv(args.datapath, sep='\t', index_col=None)
    df = pd.DataFrame(df)
    plt.figure(figsize=(6,4))
    plt.grid(True)
    plt.plot(df['p'], df['UDP_throughput'], label='UDP_throughput')
    plt.title("UDP_throughput for N=25")
    plt.xlabel("arrival rate")
    plt.legend()
    plt.savefig(args.savepath+"UDP_throughput.png")
    plt.show()

# Plot TCP_throughput vs p for all three algorithms
def plot_TCP_tp_three():  
    df1 = pd.read_csv("./results/finite/0.1_20_15_60_processor_sharing.csv", sep='\t', index_col=None)
    df2 = pd.read_csv("./results/finite/0.1_20_15_60_water_filling.csv", sep='\t', index_col=None)
    df3 = pd.read_csv("./results/finite/0.1_20_15_60_max_weight.csv", sep='\t', index_col=None)
    df1 = pd.DataFrame(df1)
    df2 = pd.DataFrame(df2)
    df3 = pd.DataFrame(df3)
    plt.figure(figsize=(6,4))   
    plt.grid(True)
    plt.plot(df1['p'], df1['TCP_throughput'], label='processor_sharing')
    plt.plot(df2['p'], df2['TCP_throughput'], label='water_filling')
    plt.plot(df3['p'], df3['TCP_throughput'], label='max_weight')
    plt.title("TCP_throughput for N=60")
    plt.xlabel("arrival rate")
    plt.legend()
    plt.savefig(args.savepath+"TCP_throughput.png")
    plt.show()


if __name__ == '__main__':
    if args.plot_p == "True":
        plot_with_varying_p()
    elif args.plot_delay == "True":
        plot_delay()
    elif args.plot_packet_drops == "True":
        plot_packet_drops()
    elif args.plot_TCP_tp == "True":
        plot_TCP_tp()
    elif args.plot_UDP_tp == "True":
        plot_UDP_tp()
    elif args.plot_TCP_tp_three == "True":
        plot_TCP_tp_three()
