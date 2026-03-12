import subprocess

test_cases = [
    ("100 4 200 1 3 2", "4"),
    ("0 1 2 3 4 5", "6"),
    ("1 1 1 1", "1"),
    ("-5 -6 -7 -8 -9", "5")
]

executable_path = "./leetcode128_test"

for input_str, expected_output in test_cases:
    print(f"Testing input: '{input_str}'")
    try:
        process = subprocess.run(
            [executable_path],
            input=input_str,

            capture_output=True,
            text=True,
            check=True
        )
        actual_output = process.stdout.strip()

        if actual_output == expected_output:
            print(f"  ✅ Passed. Expected: '{expected_output}', Got: '{actual_output}'")
        else:
            print(f"  ❌ Failed. Expected: '{expected_output}', Got: '{actual_output}'")
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
