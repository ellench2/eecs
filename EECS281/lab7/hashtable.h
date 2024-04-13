#ifndef HASHTABLE_H
#define HASHTABLE_H

// Project Identifier: 2C4A3C53CD5AD45A7BEA3AE5356A3B1622C9D04B

// INSTRUCTIONS:
// fill out the methods in the class below.

// You may assume that the key and value types can be copied and have default
// constructors.

// You can also assume that the key type has (==) and (!=) methods.

// You may assume that Hasher is a functor type with a
// size_t operator()(const Key&) overload.

// The key/value types aren't guaranteed to support any other operations.

// Do not add, remove, or change any of the class's member variables.
// The num_deleted counter is *optional*. You might find it helpful, but it
// is not required to pass the lab assignment.

// Do not change the Bucket type.

// SUBMISSION INSTRUCTIONS:
// Submit this file, by itself, in a .tar.gz.
// Other files will be ignored.

#include <cstdint>
#include <functional> // where std::hash lives
#include <vector>
#include <cassert> // useful for debugging!

// A bucket's status tells you whether it's empty, occupied, 
// or contains a deleted element.
enum class Status : uint8_t {
    Empty,
    Occupied,
    Deleted
};

template<typename K, typename V, typename Hasher = std::hash<K>>
class HashTable {
    // used by the autograder; do not change/remove.
    friend class Verifier;
public:
    // A bucket has a status, a key, and a value.
    struct Bucket {
        // Do not modify Bucket.
        Status status = Status::Empty;
        K key;
        V val;
    };

    HashTable() {

        // You can use the following to avoid implementing rehash_and_grow().
        // However, you will only pass the AG test cases ending in _C.
        // To pass the rest, start with at most 20 buckets and implement rehash_and_grow().
        //    buckets.resize(10000);
        buckets.resize(10);
    }

    size_t size() const {
        return num_elements;
    }

    // returns a reference to the value in the bucket with the key, if it
    // already exists. Otherwise, insert it with a default value, and return
    // a reference to the resulting bucket.
    V& operator[](const K& key) {
        Hasher hasher;
        if (key_finder(key) != nullptr) {
            V& found = (key_finder(key)->val);
            return found;
        }
        else {
            V default_val;
            insert(key, default_val);
            return key_finder(key)->val;
        }
    }

    // insert returns whether inserted successfully
    // (if the key already exists in the table, do nothing and return false).
    bool insert(const K& key, const V& val) {
        if (key_finder(key) != nullptr) {
            return false;
        }
        else {
            *insertion_finder(key, val) = {Status::Occupied, key, val};
            num_elements++;
            return true;
        }
    }
    // erase returns the number of items remove (0 or 1)
    size_t erase(const K& key) {
        if (key_finder(key) != nullptr) {
            key_finder(key)->status = Status::Deleted;
            num_elements--;
            num_deleted++;
            return 1;
        }
        else {
            return 0;
        }
    }

private:
    size_t num_elements = 0;
    size_t num_deleted = 0; // OPTIONAL: you don't need to use num_deleted to pass
    std::vector<Bucket> buckets;

    void rehash_and_grow() {
        std::vector<Bucket> copy_buckets = buckets;
        buckets.clear();
        buckets.resize(num_elements * 2);
        for (int i = 0; i < copy_buckets.size(); i++) {
            if (copy_buckets[i].status == Status::Occupied) {
                K copy_key = copy_buckets[i].key;
                V copy_val = copy_buckets[i].val;
                insert(copy_key, copy_val);
            }
        }

        
        
        // You can avoid implementing rehash_and_grow() by calling
        //    buckets.resize(10000);
        // in the constructor. However, you will only pass the AG test cases ending in _C.
        // To pass the rest, start with at most 20 buckets and implement rehash_and_grow().
    }

    
    Bucket* key_finder(const K& key) {
        Hasher hasher;
        for (int i = 0; i < buckets.size(); i++) {
            if (buckets[(hasher(key) + i) % buckets.size()].status == Status::Empty) {
                break;
            }
            if (buckets[(hasher(key) + i) % buckets.size()].status == Status::Occupied && buckets[(hasher(key) + i) % buckets.size()].key == key) {
                return &buckets[(hasher(key) + i) % buckets.size()];
            }
        }
        return nullptr;

    }
    Bucket* insertion_finder(const K& key, const V& val) {
        Hasher hasher;
        for (int i = 0; i < buckets.size(); i++) {
            if (buckets[(hasher(key) + i) % buckets.size()].status == Status::Empty ||
                buckets[(hasher(key) + i) % buckets.size()].status == Status::Deleted ) {
                return &buckets[(hasher(key) + i) % buckets.size()];
            }
        }
        rehash_and_grow();
        insertion_finder(key, val);
        return nullptr;
    }
};

#endif // HASHTABLE_H
