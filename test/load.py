import concurrent.futures
import requests
import time


"""
Overload the http server in order to see how it reacts
"""

URL = 'http://localhost:6969/'

NUM_REQUESTS = 10000

MAX_RETRIES = 3
TIMEOUT = 5

def make_request(url, retries=0):
    try:
        response = requests.get(url, timeout=TIMEOUT)
        return response.status_code
    except requests.exceptions.RequestException as e:
        if retries < MAX_RETRIES:
            return make_request(url, retries + 1)
        return str(e)

def main():
    start_time = time.time()
    with concurrent.futures.ThreadPoolExecutor(max_workers=50) as executor:
        futures = [executor.submit(make_request, URL) for _ in range(NUM_REQUESTS)]
        for future in concurrent.futures.as_completed(futures):
            try:
                print(future.result())
            except Exception as e:
                print(f"Error: {e}")

    print(f"Finished in {time.time() - start_time:.2f} seconds.")

if __name__ == "__main__":
    main()
