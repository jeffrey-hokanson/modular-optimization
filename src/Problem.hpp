#pragma once

#include <functional>
#include <list>
#include <optional>
#include <unordered_map>

#include <Eigen/Dense>


// Injecting into the std namespace
namespace std {
    template<typename Scalar, int Rows, int Cols>
    struct hash<Eigen::Matrix<Scalar, Rows, Cols>> {
        size_t operator()(const Eigen::Matrix<Scalar, Rows, Cols>& matrix) const {
            size_t seed = 0;
            const Scalar* data = matrix.data();
            for (size_t i = 0; i < static_cast<size_t>(matrix.size()); ++i) {
                // Boost hash_combine formula
                seed ^= std::hash<Scalar>{}(data[i]) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };
}


namespace moe {


template<typename Key, typename Value>
class LruCache
{
 public:
  LruCache(size_t cache_size) : _cache_size(cache_size)
  {
  }

  std::optional<Value> get(const Key& key) {
    auto it = _item_map.find(key);
    if (it == _item_map.end()){
      return std::nullopt;
    }
    _item_list.splice(_item_list.begin(), _item_list, it->second);
    return it->second->second;
  }

  void put(const Key& key, const Value& value) {
    auto it = _item_map.find(key);
    if(it != _item_map.end()) {
      _item_list.erase(it->second);
      _item_map.erase(it);
    }
    _item_list.push_front(std::make_pair(key, value));
    _item_map.insert(std::make_pair(key, _item_list.begin()));
    clean();
  }

  void clean()
  {
    while(_item_map.size() > _cache_size) {
      auto last_it = _item_list.end(); last_it --;
      _item_map.erase(last_it->first);
      _item_list.pop_back();
    }
  }

 private:
  size_t _cache_size;
  std::list<std::pair<Key, Value>> _item_list;
  std::unordered_map<Key, decltype(_item_list.begin())> _item_map;
};


class Problem {
 public:
  Problem(size_t cache_size = 2) : _cache_size(cache_size)
  {}

 
  // ---  Objective  ---
  void setObjective(std::function<double(Eigen::Ref<Eigen::VectorXd>)>&& obj)
  {
    _objective = obj;
  }

  std::function<double(Eigen::Ref<Eigen::VectorXd>)> getObjective() 
  {
    static LruCache<Eigen::VectorXd, double> cache(_cache_size);

    return [&](Eigen::Ref<Eigen::VectorXd> state) {
      auto value = cache.get(state);
      if (value) {
        return *value;
      }
      ++_objective_calls;
      auto obj =  _objective(state);
      cache.put(state, obj);
      return obj;
    };
  }

  size_t objectiveCalls() const {
    return _objective_calls;
  }

  // ---  Gradient  ---
  void setGradient(std::function<Eigen::VectorXd(Eigen::Ref<Eigen::VectorXd>)>&& grad)
  {
    _gradient = grad;
  }

  std::function<Eigen::VectorXd(Eigen::Ref<Eigen::VectorXd>)> getGradient() 
  {
    assert (_gradient && "Must have a gradient");
    static LruCache<Eigen::VectorXd, Eigen::VectorXd> cache(_cache_size);

    return [&](Eigen::Ref<Eigen::VectorXd> state) {
      auto value = cache.get(state);
      if (value) {
        return *value;
      }
      ++_gradient_calls;
      auto grad = (*_gradient)(state);
      cache.put(state, grad);
      return grad;
    };
  }

  size_t gradientCalls() const {
    return _gradient_calls;
  }

 private:
  std::function<double(Eigen::Ref<Eigen::VectorXd>)> _objective;
  std::optional<std::function<Eigen::VectorXd(Eigen::Ref<Eigen::VectorXd>)>> _gradient;
  std::optional<std::function<Eigen::MatrixXd(Eigen::Ref<Eigen::VectorXd>)>> _hessian;

  size_t _objective_calls = 0;
  size_t _gradient_calls = 0;
  size_t _cache_size;

};

} // namespace moe
