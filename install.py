import os
import sys
import platform
import subprocess
import shutil
from pathlib import Path

# ----------------------------------------------------
# CONFIG
# ----------------------------------------------------
PROJECT_NAME = "num"
SRC_DIR = Path("src")
TOKENIZER_DIR = SRC_DIR / "tokenizer"
INTERPRETER_DIR = SRC_DIR / "interpreter"

WIN_INSTALL_DIR = Path(os.getenv("LOCALAPPDATA", "")) / "Programs" / PROJECT_NAME
UNIX_INSTALL_DIR = Path("/usr/local/bin") / PROJECT_NAME

# ----------------------------------------------------
# UTILS
# ----------------------------------------------------
def run(cmd):
    print(">>", " ".join(cmd))
    subprocess.check_call(cmd)

def compiler_exists(name):
    return shutil.which(name) is not None

def choose_compiler():
    system = platform.system().lower()

    if system == "windows":
        if compiler_exists("clang++"):
            return "clang++"
        if compiler_exists("g++"):
            return "g++"
        if compiler_exists("cl"):
            return "cl"
        return None

    # Linux/macOS
    if compiler_exists("clang++"):
        return "clang++"
    if compiler_exists("g++"):
        return "g++"
    return None

# ----------------------------------------------------
# WINDOWS PATH ADDER
# ----------------------------------------------------
def add_to_windows_path(path):
    try:
        import winreg

        with winreg.OpenKey(
            winreg.HKEY_CURRENT_USER,
            r"Environment",
            0,
            winreg.KEY_ALL_ACCESS
        ) as key:

            try:
                existing = winreg.QueryValueEx(key, "PATH")[0]
            except FileNotFoundError:
                existing = ""

            paths = existing.split(";")

            if path not in paths:
                new_path = existing + (";" if existing else "") + path
                winreg.SetValueEx(key, "PATH", 0, winreg.REG_EXPAND_SZ, new_path)
                print("\n Added NUM install directory to PATH.")
                print("  Restart your terminal for it to take effect.")
            else:
                print("\nNUM path already exists in PATH.")

    except Exception as e:
        print("\n[WARNING] Cannot auto-add to PATH:", e)
        print("Add it manually:", path)

# ----------------------------------------------------
# COMPILATION
# ----------------------------------------------------
def compile_program(compiler, output_path):
    cpp_files = list(map(str, SRC_DIR.glob("*.cpp")))
    cpp_files += [str(TOKENIZER_DIR / "tokenizer.cpp")]
    cpp_files += [str(INTERPRETER_DIR / "interpreter.cpp")]

    if compiler == "cl":
        cmd = ["cl"] + cpp_files + ["/EHsc", "/O2", "/Fe:" + str(output_path)]
    else:
        cmd = [compiler, "-O3", "-g", "-o", str(output_path)] + cpp_files

    run(cmd)

# ----------------------------------------------------
# INSTALLER
# ----------------------------------------------------
def install_binary(binary_path):
    system = platform.system().lower()

    if system == "windows":
        WIN_INSTALL_DIR.mkdir(parents=True, exist_ok=True)
        output = WIN_INSTALL_DIR / "num.exe"
        shutil.copy(binary_path, output)
        print(f"\n Installed NUM → {output}")

        add_to_windows_path(str(WIN_INSTALL_DIR))
        return

    # Linux / macOS
    try:
        shutil.copy(binary_path, UNIX_INSTALL_DIR)
        print(f"\n Installed NUM → {UNIX_INSTALL_DIR}")
    except PermissionError:
        print("\n[ERROR] Permission denied.")
        print("Run this manually:")
        print(f"  sudo cp {binary_path} {UNIX_INSTALL_DIR}")

# ----------------------------------------------------
# UNINSTALL
# ----------------------------------------------------
def uninstall():
    system = platform.system().lower()

    if system == "windows":
        if WIN_INSTALL_DIR.exists():
            shutil.rmtree(WIN_INSTALL_DIR)
            print(" NUM uninstalled from Windows.")
        return

    if UNIX_INSTALL_DIR.exists():
        try:
            UNIX_INSTALL_DIR.unlink()
            print(" NUM removed from /usr/local/bin")
        except PermissionError:
            print("[ERROR] Need sudo:")
            print(f"  sudo rm {UNIX_INSTALL_DIR}")

# ----------------------------------------------------
# MAIN
# ----------------------------------------------------
def main():
    if "--uninstall" in sys.argv:
        uninstall()
        return

    print("\n==============================")
    print("       NUM Installer")
    print("==============================\n")

    compiler = choose_compiler()
    if compiler is None:
        print(" No C++ compiler found.")
        print("Install clang++ or g++ (Linux/macOS)")
        print("Or install Visual Studio Build Tools or MinGW (Windows)")
        return

    print(" Using compiler:", compiler)

    BUILD_DIR = Path("bin")
    BUILD_DIR.mkdir(exist_ok=True)

    binary_output = BUILD_DIR / (
        "num.exe" if platform.system().lower() == "windows" else "num"
    )

    print("\nCompiling NUM...")
    compile_program(compiler, binary_output)

    print("\nInstalling NUM...")
    install_binary(binary_output)

    print("\nDone! You can now run:\n")
    print("  num --version\n")
    print("To uninstall:")
    print("  python3 install.py --uninstall\n")

if __name__ == "__main__":
    main()
