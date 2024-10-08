#pragma once

#include "types.h"
#include <map>
#include <stack>

template<typename T>
static T* get_device(const int N, bool with_zero = false)
{
    T* gpu = nullptr;
    SAFE_CALL(cudaMalloc((void**)&gpu, sizeof(T) * N));
    if (with_zero)
        SAFE_CALL(cudaMemset(gpu, 0, sizeof(T) * N));
    return gpu;
}

static std::map<int, std::stack<storage*>> pools;
static std::map<int, int> counts;
static std::map<int, int> minCounts;
static bool debugPrint = false;

static storage* get_pool(const int N, bool with_zero = false)
{
    auto it = pools.find(N);
    
    if (it == pools.end()) {
        it = pools.insert(it, std::make_pair(N, std::stack<storage*>()));
        counts[N] = 0;
    }
    
    if (it->second.empty())
    {
        counts[N]++;
        if (debugPrint)
            printf(" ==> get from pool %d with_zero %d\n", N, with_zero);
        return get_device<storage>(N, with_zero);
    }
    else 
    {
        auto pointer = it->second.top();
        it->second.pop();
        
        if (debugPrint)
            minCounts[N] = std::min(minCounts[N], (int)it->second.size());
        //printf(" == reuse from pool %d\n", N);
        return pointer;
    }
}

static void release_pool(storage* &pointer, const int N)
{
    if (pointer == nullptr)
        return;

    auto it = pools.find(N);
    if (it == pools.end())
    {
        printf("error in release_pool\n");
        exit(-1);
    }
    
    //printf(" == release pool %d\n", N);
    it->second.push(pointer);
    pointer = nullptr;
}