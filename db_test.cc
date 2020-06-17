#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <limits>
#include <map>

#include "db_core.h"

static double MapBenchmark(int dimension) {
  uint64_t seed = time(nullptr);

  RandomGenerator xorshift;
  xorshift.Seed(seed);
  size_t N = dimension;

  std::vector<double> keys(N);
  std::vector<double> vals(N);
  for (size_t i = 0; i < N; i++) {
    keys[i] = xorshift.Uniform();
    vals[i] = xorshift.Uniform();
  }

  Map<double, double> tree;

  auto t1 = std::chrono::high_resolution_clock::now();

  for (size_t i = 0; i < N; i++) {
    tree.Put(keys[i], vals[i]);
  }

  MapIterator<double, double> it;
  for (size_t i = 0; i < N; i++) {
    it = tree.Find(keys[xorshift.Uint64() % N]);
  }

  for (size_t i = 0; i < N; i++) {
    tree.Erase(tree.Begin());
  }

  auto t2 = std::chrono::high_resolution_clock::now();

  auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

  std::chrono::duration<double, std::milli> fp_ms = t2 - t1;

  return fp_ms.count();
}

static double StandardMapBenchmark(int dimension) {
  uint64_t seed = time(nullptr);

  RandomGenerator xorshift;
  xorshift.Seed(seed);
  size_t N = dimension;

  std::vector<double> keys(N);
  std::vector<double> vals(N);
  for (size_t i = 0; i < N; i++) {
    keys[i] = xorshift.Uniform();
    vals[i] = xorshift.Uniform();
  }

  std::map<double, double> tree;

  auto t1 = std::chrono::high_resolution_clock::now();

  for (size_t i = 0; i < N; i++) {
    tree.insert(std::make_pair(keys[i], vals[i]));
  }

  std::map<double, double>::iterator it;
  for (size_t i = 0; i < N; i++) {
    it = tree.find(keys[xorshift.Uint64() % N]);
  }

  for (size_t i = 0; i < N; i++) {
    tree.erase(tree.begin());
  }

  auto t2 = std::chrono::high_resolution_clock::now();

  auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

  std::chrono::duration<double, std::milli> fp_ms = t2 - t1;

  return fp_ms.count();
}

static double MultimapBenchmark(int dimension) {
  Multimap<long, long> tree;

  uint64_t seed = time(nullptr);

  RandomGenerator xorshift;
  xorshift.Seed(seed);
  size_t N = dimension;

  std::vector<long> keys(N);
  std::vector<long> vals(N);
  for (size_t i = 0; i < N; i++) {
    keys[i] = xorshift.Uint64() % (N / 10);
    vals[i] = xorshift.Uint64() % (N / 10);
  }

  auto t1 = std::chrono::high_resolution_clock::now();

  for (size_t i = 0; i < N; i++) {
    tree.Put(keys[i], vals[i]);
  }

  MultimapIterator<long, long> it;
  for (size_t i = 0; i < N; i++) {
    it = tree.Find(keys[xorshift.Uint64() % N]);
  }

  for (size_t i = 0; i < N; i++) {
    tree.Erase(tree.Begin());
  }

  auto t2 = std::chrono::high_resolution_clock::now();

  auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

  std::chrono::duration<double, std::milli> fp_ms = t2 - t1;

  return fp_ms.count();
}

static double StandardMultimapBenchmark(int dimension) {
  std::multimap<long, long> tree;
  uint64_t seed = time(nullptr);

  RandomGenerator xorshift;
  xorshift.Seed(seed);
  size_t N = dimension;

  std::vector<long> keys(N);
  std::vector<long> vals(N);
  for (size_t i = 0; i < N; i++) {
    keys[i] = xorshift.Uint64() % (N / 10);
    vals[i] = xorshift.Uint64() % (N / 10);
  }

  auto t1 = std::chrono::high_resolution_clock::now();

  for (size_t i = 0; i < N; i++) {
    tree.insert(std::make_pair(keys[i], vals[i]));
  }

  std::multimap<long, long>::iterator it;
  for (size_t i = 0; i < N; i++) {
    it = tree.find(keys[xorshift.Uint64() % N]);
  }

  for (size_t i = 0; i < N; i++) {
    tree.erase(tree.begin());
  }

  auto t2 = std::chrono::high_resolution_clock::now();

  auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

  std::chrono::duration<double, std::milli> fp_ms = t2 - t1;

  return fp_ms.count();
}

static void MapSerialization(int powers) {
  Map<double, double> tree;

  uint64_t seed = time(nullptr);

  RandomGenerator xorshift;
  xorshift.Seed(seed);
  size_t N = pow(10, powers);

  std::vector<double> keys(N);
  std::vector<double> vals(N);
  for (size_t i = 0; i < N; i++) {
    keys[i] = xorshift.Uniform();
    vals[i] = xorshift.Uniform();
  }

  for (size_t i = 0; i < N; i++) {
    tree.Put(keys[i], vals[i]);
  }

  std::cout << "save tree to tree1.bin" << std::endl;
  tree.Save("tree1.bin");
  std::cout << "clear tree" << std::endl;
  tree.Clear();
  std::cout << "load tree from tree1.bin" << std::endl;
  tree.Load("tree1.bin");
  std::cout << "save tree to tree2.bin" << std::endl;
  tree.Save("tree2.bin");
  std::cout << "clear tree" << std::endl;
  tree.Clear();

  std::cout << "load and show tree1.bin" << std::endl;
  tree.Load("tree1.bin");
  size_t every = pow(10, powers - 1);
  size_t i = 0;
  for (MapIterator<double, double> it = tree.Begin(); it != tree.End(); it++) {
    if (i++ % every == 0) {
      std::cout << it.GetKey() << "\t" << it.GetValue() << std::endl;
    }
  }
  tree.Clear();

  std::cout << "load and show tree2.bin" << std::endl;
  tree.Load("tree2.bin");
  i = 0;
  for (MapIterator<double, double> it = tree.Begin(); it != tree.End(); it++) {
    if (i++ % every == 0) {
      std::cout << it.GetKey() << "\t" << it.GetValue() << std::endl;
    }
  }
  tree.Clear();
}

static void MultimapSerialization(int powers) {
  Multimap<double, double> tree;

  uint64_t seed = time(nullptr);

  RandomGenerator xorshift;
  xorshift.Seed(seed);
  size_t N = pow(10, powers);

  std::vector<double> keys(N);
  std::vector<double> vals(N);
  for (size_t i = 0; i < N; i++) {
    keys[i] = xorshift.Uniform();
    vals[i] = xorshift.Uniform();
  }

  for (size_t i = 0; i < N; i++) {
    tree.Put(keys[i], vals[i]);
  }

  std::cout << "save tree to multitree1.bin" << std::endl;
  tree.Save("multitree1.bin");
  std::cout << "clear tree" << std::endl;
  tree.Clear();
  std::cout << "load tree from multitree1.bin" << std::endl;
  tree.Load("multitree1.bin");
  std::cout << "save tree to multitree2.bin" << std::endl;
  tree.Save("multitree2.bin");
  std::cout << "clear tree" << std::endl;
  tree.Clear();

  std::cout << "load and show multitree1.bin" << std::endl;
  tree.Load("multitree1.bin");
  size_t every = pow(10, powers - 1);
  size_t i = 0;
  for (MultimapIterator<double, double> it = tree.Begin(); it != tree.End();
       it++) {
    if (i++ % every == 0) {
      std::cout << it.GetKey() << "\t" << it.GetValue() << std::endl;
    }
  }
  tree.Clear();

  std::cout << "load and show multitree2.bin" << std::endl;
  tree.Load("multitree2.bin");
  i = 0;
  for (MultimapIterator<double, double> it = tree.Begin(); it != tree.End();
       it++) {
    if (i++ % every == 0) {
      std::cout << it.GetKey() << "\t" << it.GetValue() << std::endl;
    }
  }
  tree.Clear();
}

static void StringMapSerialization(int powers) {
  Map<std::string, std::string> tree;
  uint64_t seed = time(nullptr);

  RandomGenerator xorshift;
  xorshift.Seed(seed);
  size_t N = pow(10, powers);

  std::vector<std::string> keys(N);
  std::vector<std::string> values(N);
  for (size_t i = 0; i < N; i++) {
    keys[i] = xorshift.Uuid();
    values[i] = xorshift.Uuid();
  }

  for (size_t i = 0; i < N; i++) {
    tree.Put(keys[i], values[i]);
  }

  size_t every = pow(10, powers - 1);
  size_t i = 0;
  for (MapIterator<std::string, std::string> it = tree.Begin();
       it != tree.End(); it++) {
    if (i++ % every == 0) {
      std::cout << it.GetKey() << "\t" << it.GetValue() << std::endl;
    }
  }

  MapIterator<std::string, std::string> iit;
  for (size_t i = 0; i < N; i++) {
    iit = tree.Find(keys[xorshift.Uint64() % N]);
  }

  i = 0;
  for (size_t i = 0; i < N; i++) {
    tree.Erase(tree.Begin());
  }

  for (size_t i = 0; i < N; i++) {
    tree.Put(keys[i], values[i]);
  }

  i = 0;
  for (MapIterator<std::string, std::string> it = tree.Begin();
       it != tree.End(); it++) {
    if (i++ % every == 0) {
      std::cout << it.GetKey() << "\t" << it.GetValue() << std::endl;
    }
  }

  std::cout << "save tree to stringtree1.bin" << std::endl;
  tree.Save("stringtree1.bin");
  std::cout << "clear tree" << std::endl;
  tree.Clear();
  std::cout << "load tree from stringtree1.bin" << std::endl;
  tree.Load("stringtree1.bin");
  std::cout << "save tree to stringtree2.bin" << std::endl;
  tree.Save("stringtree2.bin");
  std::cout << "clear tree" << std::endl;
  tree.Clear();

  std::cout << "load and show stringtree1.bin" << std::endl;
  tree.Load("stringtree1.bin");
  i = 0;
  for (MapIterator<std::string, std::string> it = tree.Begin();
       it != tree.End(); it++) {
    if (i++ % every == 0) {
      std::cout << it.GetKey() << "\t" << it.GetValue() << std::endl;
    }
  }
  tree.Clear();

  std::cout << "load and show stringtree2.bin" << std::endl;
  tree.Load("stringtree2.bin");
  i = 0;
  for (MapIterator<std::string, std::string> it = tree.Begin();
       it != tree.End(); it++) {
    if (i++ % every == 0) {
      std::cout << it.GetKey() << "\t" << it.GetValue() << std::endl;
    }
  }
  tree.Clear();
}

int main(int argc, char **argv) {

  size_t max_power = 5;
  std::cout << "# my_map, map, my_multimap, multimap" << std::endl;
  for (size_t dim = 1e1; dim <= pow(10, max_power); dim *= 2) {
    double my_map_average = 0.0;
    double map_average = 0.0;
    double my_multimap_average = 0.0;
    double multimap_average = 0.0;
    size_t count = 1;
    for (size_t i = 0; i < count; i++) {
      my_map_average += MapBenchmark(dim);
      map_average += StandardMapBenchmark(dim);
      my_multimap_average += MultimapBenchmark(dim);
      multimap_average += StandardMultimapBenchmark(dim);
    }
    my_map_average /= count;
    map_average /= count;
    my_multimap_average /= count;
    multimap_average /= count;
    std::cout << dim << "\t" << my_map_average << "\t" << map_average << "\t"
              << my_multimap_average << "\t" << multimap_average << std::endl;
  }

  MapSerialization(max_power);
  MultimapSerialization(max_power);
  StringMapSerialization(max_power);

  return 0;
}