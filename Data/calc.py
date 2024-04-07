def count_unique_words(file_name):
    unique_words = set()
    
    with open(file_name, 'r') as file:
        for line in file:
            words = line.split()
            for word in words:
                unique_words.add(word)
    
    return len(unique_words)

file_name = "/home/oleg/Documents/MyAsmTasks/HashTable/Data/CleanFile.txt"  # Укажите здесь имя файла, который нужно обработать
unique_word_count = count_unique_words(file_name)
print(f"Количество различных слов в файле {file_name}: {unique_word_count}")
