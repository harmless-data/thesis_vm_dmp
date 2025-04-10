import subprocess
import re 
from enum import Enum
form dataclass import dataclass

class LogCode(Enum):
    REL = "rel"
    OPN = "opn"
    LOD = "lod"
    DEV = "dev"
    DMA = "dma"

@dataclass
def LogEntry:
    logged_at: int
    code: LogCode
    ktime: int

def dmesg():
    log = subprocess.run(["dmesg"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True, executable="/bin/bash", text=True, check=True)
    return log.stdout

def filter_for_keyword(log, keyword):
    return "\n".join([line for line in log.splitlines() if keyword in line])

def fetch_values(filtered_log):
    pattern = re.compile(r"logmodule:\s+(\w{3})\s+.*?\[ktime:\s+(\d+)\]")
    matches = pattern.findall(filtered_log)
    return matches


if __name__ == "__main__":

    log = dmesg()
    filtered = filter_for_keyword(dmesg(), "logmodule:")
    values = fetch_values(filtered)

    print(values)

