from pathlib import Path
import json

BANK_DIRECTORY = "banks/"


def convert(bank_number):
    with open(f"{BANK_DIRECTORY}bank_{bank_number}.json", "r") as b:
        bank = json.load(b)
    bank_info = {
        "name": bank["name"],
        "version": bank["version"],
    }
    output_dir = f"{BANK_DIRECTORY}bank_{bank_number}"
    Path(output_dir).mkdir(exist_ok=True)
    with open(f"{output_dir}/bank.json", "w") as b:
        json.dump(bank_info, b)
    preset_number = 0
    for p in bank["presets"]:
        with open(f"{output_dir}/preset_{preset_number}.json", "w") as p_file:
            json.dump(p, p_file)
        preset_number += 1


if __name__ == "__main__":
    for i in range(1, 11):
        convert(i)
