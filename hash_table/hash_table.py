class Node:
    def __init__(self, key, value):
        self.key = key
        self.value = value
        self.next = None


class HashTable:
    def __init__(self, initial_capacity=8, load_factor=0.7):
        self.capacity = initial_capacity
        self.size = 0
        self.table = [None]*initial_capacity
        self.load_factor = load_factor

    def __str__(self):
        elements = []
        for i in range(self.capacity):
            current = self.table[i]
            while current:
                elements.append((current.key, current.value))
                current = current.next
        return str(elements)

    def __len__(self):
        return self.size

    def __contains__(self, key):
        try:
            self.search(key)
            return True
        except KeyError:
            return False

    def _hash(self, key):
        return hash(key) % self.capacity

    def _resize(self):
        old_table = self.table
        self.capacity *= 2
        self.size = 0
        self.table = [None]*self.capacity

        for node in old_table:
            current = node
            while current:
                self.insert(current.key, current.value)
                current = current.next

    def insert(self, key, value):
        if self.size / self.capacity > self.load_factor:
            self._resize()

        index = self._hash(key)

        if self.table[index] is None:
            self.table[index] = Node(key, value)
            self.size += 1
        else:
            current = self.table[index]
            while current:
                if current.key == key:
                    current.value = value
                    return
                current = current.next

            # Else create a node and insert it at the head
            new_node = Node(key, value)
            new_node.next = self.table[index]
            self.table[index] = new_node
            self.size += 1

    def search(self, key):
        index = self._hash(key)
        current = self.table[index]

        while current:
            if current.key == key:
                return current.value
            current = current.next
        raise KeyError(key)

    def remove(self, key):
        index = self._hash(key)
        previous = None
        current = self.table[index]

        while current:
            if current.key == key:
                if previous:
                    previous.next = current.next
                else:
                    self.table[index] = current.next
                    self.size -= 1
                    return
            previous = current
            current = current.next

        raise KeyError(key)


def test_hash_table():
    ht = HashTable(4)
    print(ht)

    ht.insert("apple", 3)
    ht.insert("banana", 2)
    print(ht)
    assert ht.capacity == 4

    ht.insert("cherry", 5)
    ht.insert("blueberry", 5)
    print(ht)
    size = 4
    assert len(ht) == size
    assert ht.capacity == 8

    assert "apple" in ht
    assert "durian" not in ht
    assert ht.search("banana") == 2

    ht.insert("banana", 4)
    print(ht)
    assert ht.search("banana") == 4
    assert len(ht) == size

    ht.remove("apple")
    size -= 1
    print(ht)
    assert len(ht) == size


if __name__ == "__main__":
    test_hash_table()
    print("All tests passed !")
