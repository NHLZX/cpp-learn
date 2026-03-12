import subprocess

expected_output = """
MyClass constructor: 100
MyClass constructor: 200
obj1 value: 100
obj2 value: 200
MyClass destructor: 100
MyClass destructor: 200
""".strip()

executable_path = "./alloctor_test"

print(f"Testing: {executable_path}")
try:
    process = subprocess.run(
        [executable_path],
        capture_output=True,
        text=True,
        check=True
    )
    actual_output = process.stdout.strip()

    if actual_output == expected_output:
        print(f"  ✅ Passed. Output matches expected.\nExpected:\n'{expected_output}'\nActual:\n'{actual_output}'")
    else:
        print(f"  ❌ Failed. Output mismatch.\nExpected:\n'{expected_output}'\nActual:\n'{actual_output}'")
        print(f"  Stderr: {process.stderr}")
except subprocess.CalledProcessError as e:
    print(f"  ❌ Test failed due to an error during execution.")
    print(f"  Command: {e.cmd}")
    print(f"  Return Code: {e.returncode}")
    print(f"  Stdout: {e.stdout}")
    print(f"  Stderr: {e.stderr}")
except FileNotFoundError:
    print(f"  ❌ Error: Executable not found at {executable_path}. Please ensure it is compiled and in the correct directory.")
print("-" * 30)
