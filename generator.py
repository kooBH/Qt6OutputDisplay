from time import sleep
txt_path = "./output/result.txt"
count = 0
while(1):
    sleep(0.5)
    if count % 3 == 0:
        with open(txt_path, "w",encoding='utf8') as f:
            f.write("행복함\n")
            f.write("happy")
            f.close()
    elif count % 3 == 1:
        with open(txt_path, "w",encoding='utf8') as f:
            f.write("그냥저냥\n")
            f.write("neutral")
            f.close()
    else:
        with open(txt_path, "w",encoding='utf8') as f:
            f.write("화가난다..\n")
            f.write("angry")
            f.close()
    count += 1