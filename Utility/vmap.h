#ifndef VMAP_H
#define VMAP_H

//VMap is a container that behaves mostly like std::map, but stores data like an std::vector
//its empty size is a single pointer
//insertion and deletion is O(n), access is O(log(n))
//while the complexity is worse than for std::map, the cache locality is better
template <class Key, class Value>
struct VMap
{
	using Size_type = unsigned int;
	VMap()
		:data(nullptr)
	{}
	bool empty() const noexcept{
		return data;
	}
	Size_type size() const noexcept{
		if (!data)
			return 0;
		return static_cast<const Size_type *>(data)[0];
	}
	Size_type capacity() const noexcept{
		if (!data)
			return 0;
		return static_cast<const Size_type *>(data)[1];
	}

private:
	void *data;
};

#endif // VMAP_H
