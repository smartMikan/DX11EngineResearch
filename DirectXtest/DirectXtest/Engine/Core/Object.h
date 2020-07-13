#pragma once
//Learned From WeckedEngine/wiECS
//https://wickedengine.net/2019/09/29/entity-component-system/


#include<stdint.h>
#include<assert.h>
#include<vector>
#include<unordered_map>

//Global Component ID
using Object = uint_fast64_t;

static const Object NULLID = 0;

inline Object CreateGOID() {
	static Object next = 0;
	return ++next;
}

///Manage each type of component for each object
//e.g.
//Object 1,2,3.......
//Object 1 has Transform,RigidBody,Collider
//Object 2 has Transfrom
//Object 3 has Collider

//And we will have
//ComponentManager<Transfrom>
//ComponentManager<RigidBody>
//ComponentManager<Collider>

//And ComponentManager<Transfrom> will have any array of all Transfrom Class 
//and a list point that Object 1 and Object 2 have the Transform Class 
//and a map point out which Object address to which Transform Class's Address
//Spesific to Transform I will follow weck engine's work and try to Build my Hierachy System too.(and delete this lol)





template<typename Component>
class ComponentManager
{
public:
	ComponentManager(size_t reservedCount = 0) 
	{
		objects.reserve(reservedCount);
		objects.reserve(reservedCount);
		objmap.reserve(reservedCount);
	}

	//if contain sameobj
	inline bool Contains(Object _obj) const
	{
		return objmap.find(_obj) != objmap.end();
	}


	//Create a component in an explicit object
	inline Component& Create(Object _obj)
	{
		//Reserve 0
		assert(_obj != NULLID,"GID Must Start Above 0!");

		assert(objmap.find(obj) == objmap.end(), "One type of Componet fetch Per Object!");

		assert(objects.size() == components.size(), "Objects overflow");
		assert(objmap.size() == components.size(), "Objectsmap overflow");

		//update map to component lase address
		objmap[_obj] = components.size();

		//add component at the address
		components.push_back(Component());

		//add object(id)
		objects.push_back(_obj);

		return components.back();
	}

	inline void Remove(Object _obj) 
	{
		auto it = objmap.find(_obj);
		//if Contain _obj
		if (it != objmap.end())
		{
			//direct to this obj's address
			const size_t index = it->second;

			//direct object id
			const Object object = objects[index];

			//contains address
			if (index < components.size() - 1)
			{
				//swap components at the address with last address;
				components[index] = std::move(components.back());

				//retarget current count(position) of obj's id with last obj's id(move last obj's position to current position)
				objects[index] = objects.back();
				//remap the last obj's address
				objmap[objects[index]] = index;
			}

			//delete last element
			components.pop_back();
			objects.pop_back();
			objmap.erase(_obj);
		}
	}

	//keep parent-child relationship
	inline void Remove_KeepSorted(Object _obj) 
	{
		auto it = objmap.find(_obj);
		if (it != objmap.end())
		{
			//direct to this obj's address
			const size_t index = it->second;

			//direct object id
			const Object object = objects[index];
			
			//contains address
			if (index < components.size() - 1) 
			{

				///
				//move elements' address after this obj left by one element(and swap this components address back one by one)
				for (size_t i = index + 1 ,ie = components.size(); i < ie; i++)
				{
					components[i - 1] = std::move(components[i]);
				}
				//update id and direct map after elements move (swap back one by one)
				for (size_t i = index +1 ,ie = objects.size(); i < ie; i++)
				{
					objects[i - 1] = objects[i];
					objmap[objects[i - 1]] = i - 1;
				}
			}
			
			//delete last one
			components.pop_back();
			objects.pop_back();
			objmap.erase(object);

		}
	}

	//MoveObject Keep Sort
	inline void MoveItem(size_t _index_from,size_t _index_to) 
	{
		assert(_index_from < GetCount(), "Does not Contain This IndexKey!");
		assert(_index_to < GetCount(), "Does not Contain This IndexKey!");

		if (_index_from == _index_to) 
		{
			return;
		}

		//Get a Address Backup
		Component component = std::move(components[index_from]);
		Object object = objects[_index_from];

		//determine direction
		const int direction = _index_from < _index_to ? 1 : -1;
		for (size_t i = _index_from; i != _index_to; i+= direction)
		{
			const size_t next = i + direction;
			components[i] = std::move(components[next]);
			objects[i] = objects[next];
			objmap [objects[i]]= i;
		}

		//swap backup to target address
		components[_index_to] = std::move(component);
		objects[_index_to] = object;
		objmap[object] = _index_to;
	}


	//Get -r/-w access to the component of explicit object
	inline Component* GetComponent(Object _obj)
	{
		auto it = objmap.find(_obj);
		if (it != objmap.end())
		{
			return &components[it->second];
		}
		return nullptr;
	}


	//Get an read-only access to the component of explicit object
	inline const Component* GetComponent(Object _obj) const
	{
		const auto it = objmap.find(_obj);
		if (it != objmap.end())
		{
			return &components[it->second];
		}
		return nullptr;
	}



	//Get the Address
	inline size_t GetIndex(Object _obj) const
	{
		const auto it = objmap.find(_obj);
		if (it != objmap.end())
		{
			return it->second;
		}
		//get uint64 MAX
		return ~0;
	}

	//Get ComponentCount
	inline size_t GetCount() const { return components.size(); }
	
	//Get a object(id)
	inline Object GetObject(size_t index) const { return objects[index]; }

	//Get a Component(-rw) use ComponentManager[]
	inline Component& operator[](size_t index) { return components[index]; }

	//Get a Component(-r-) use ComponentManager[]
	inline const Conponent& operator[](size_t index) const {return components[index]; }

	//reset
	inline void ResetAll() 
	{
		objects.clear();
		objects.clear();
		objmap.clear();
	}

	//deep copy
	inline void Copy(const ComponentManager<Component>& rhs) 
	{
		ResetAll();
		objects = rhs.objects;
		objects = rhs.objects;
		objmap = rhs.objmap;
	}

	//merge two manager
	inline void Merge(ComponentManager<Component>& other)
	{
		components.reserve(GetCount() + other.GetCount());
		objects.reserve(GetCount() + other.GetCount());
		objmap.reserve(GetCount() + other.GetCount());
		for (size_t i = 0; i < other.GetCount(); i++)
		{
			Object obj = other.objects[i];
			assert(!Contains(obj),"Has a same object!");
			objects.push_back(obj);
			objmap[obj] = components.size();
			//steal component adress
			components.push_back(std::move(other.components[i]));
		}
		//Clear
		other.ResetAll();
	}

private:
	//Component
	std::vector<Component> components;
	
	//EachObjWith Unique Id
	std::vector<Object> objects;

	//direct to the obj address(local address)
	std::unordered_map<Object, size_t> objmap;

	//Block non-component copy
	ComponentManager(const ComponentManager&) = delete;
};

