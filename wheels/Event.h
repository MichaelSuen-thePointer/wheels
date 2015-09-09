#pragma once

#ifndef EVENT_H
#define EVENT_H

namespace pl
{

#define DEFINE_NONVOID_EVENT(Name, Return, Names, Params)								\
	class IEvent##Name : public Object													\
	{																					\
	public:																				\
		virtual Return Invoke Params = 0;												\
		virtual void GetElement(void*& A, void*& B) = 0;								\
	};																					\
																						\
	class EventNormal##Name : public IEvent##Name										\
	{																					\
	public:																				\
		typedef Return (*Handler) Params;												\
	protected:																			\
		Handler _Handler;																\
	public:																				\
		EventNormal##Name(Handler h);													\
		Return Invoke Params override;													\
		void GetElement(void*& A, void*& B) override;									\
	};																					\
																						\
	template<typename TClass>															\
	class EventMember##Name : public IEvent##Name										\
	{																					\
	public:																				\
		typedef Return (TClass::*Handler) Params;										\
	protected:																			\
		TClass* _Owner;																	\
		Handler _Handler;																\
	public:																				\
		EventMember##Name(TClass* o, Handler h)											\
			: _Owner(o)																	\
			, _Handler(h)																\
		{}																				\
		Return Invoke Params override													\
		{																				\
			return (_Owner->*_Handler) Names;											\
		}																				\
		void GetElement(void*& A, void*& B) override									\
		{																				\
			A = static_cast<void*>(_Owner);												\
			B = static_cast<void*>(*static_cast<int*>(&_Handler));						\
		}																				\
	};																					\
																						\
	class Name : public Object															\
	{																					\
	protected:																			\
		typedef std::vector<IEvent##Name*> HandlerList;									\
		HandlerList _Handlers;															\
		int Find(IEvent##Name* Handler);												\
		int Bind(IEvent##Name* Handler);												\
		void Unbind(IEvent##Name* Handler);												\
	public:																				\
		Name();																			\
		~Name();																		\
		void Bind(Event_Normal_##Name::Handler Handler);								\
		void Unbind(Event_Normal_##Name::Handler Handler);								\
		template <typename TClass>														\
		void Bind(TClass* Owner, typename EventMember##Name<TClass>::Handler Handler)	\
		{																				\
			Bind(new EventMember##Name<TClass>(Owner, Handler));						\
		}																				\
		template <typename TClass>														\
		void Unbind(TClass* Owner, typename EventMember##Name<TClass>::Handler Handler)	\
		{																				\
			Unbind(new EventMember##Name<TClass>(Owner, Handler));						\
		}																				\
		Return operator() Params;														\
	};

#define IMPLEMENT_NONVOID_EVENT(Name, Return, Names, Params)							\
	EventNormal##Name::EventNormal##Name(Handler h)										\
		: _Handler(h)																	\
	{}																					\
	Return EventNormal##Name::Invoke Params												\
	{																					\
		return _Handler Names;															\
	}																					\
	void EventNormal##Name::GetElement(void*& A, void*& B)								\
	{																					\
		A = nullptr;																	\
		B = static_cast<void*>(_Handler);												\
	}																					\
	int Name::Find(IEvent##Name* Handler)												\
	{																					\
		void* NewA;																		\
		void* NewB;																		\
		Handler->GetElement(NewA, NewB);													\
		for (int i = 0; i < _Handlers.size(); i++)										\
		{																				\
			_Handlers->GetElement(OldA, OldB);											\
			if (NewA == OldA && NewB == OldB)											\
			{																			\
				return i;																\
			}																			\
		}																				\
		return -1;																		\
	}																					\
	void Name::Bind(IEvent##Name* Handler)												\
	{																					\
		int Index = Find(Handler);														\
		if (Index != -1)																	\
		{																				\
			delete Handler;																\
		}																				\
		else																			\
		{																				\
			_Handlers.push_back(Handler);												\
		}																				\
	}																					\
	void Name::Unbind(IEvent##Name* Handler)											\
	{																					\
		int Index = Find(Handler);														\
		if (Index != -1)																	\
		{																				\
			delete _Handlers[i];														\
			_Handlers.erase(_Handlers.begin() + i);										\
		}																				\
		delete Handler;																	\
	}																					\
	Name::Name(){}																		\
	Name::~Name()																		\
	{																					\
		for (auto& elem : _Handlers)													\
		{																				\
			delete elem;																\
		}																				\
	}																					\
	void Name::Bind(EventNormal##Name::Handler Handler)									\
	{																					\
		Bind(new EventNormal##Name(Handler));											\
	}																					\
	void Name::Unbind(EventNormal##Name::Handler Handler)								\
	{																					\
		Unbind(new EventNormal##Name(Handler));											\
	}																					\
	Return Name::operator() Params														\
	{																					\
		Return Result = Return();														\
		for (int i = 0; i < _Handlers.size(); i++)										\
		{																				\
			Result = _Handlers[i]->Invoke Names;											\
		}																				\
		return Result;																	\
	}

#define DEFINE_VOID_EVENT(Name, Names, Params)											\
	class IEvent##Name : public Object													\
	{																					\
	public:																				\
		virtual void Invoke Params = 0;													\
		virtual void GetElement(void*& A, void*& B) = 0;								\
	};																					\
																						\
	class EventNormal##Name : public IEvent##Name										\
	{																					\
	public:																				\
		typedef void (*Handler) Params ;												\
	protected:																			\
		Handler _Handler;																\
	public:																				\
		EventNormal##Name(Handler h);													\
		void Invoke Params override;													\
		void GetElement(void*& A, void*& B) override;									\
	};																					\
																						\
	template <typename TClass>															\
	class EventMember##Name : public IEvent##Name										\
	{																					\
	public:																				\
		typedef void (TClass::*Handler) Params ;										\
	protected:																			\
		TClass* _Owner;																	\
		Handler _Handler;																\
	public:																				\
		EventMember##Name(TClass* o, Handler h)										\
			: _Owner(o)																	\
			, _Handler(h)																\
		{}																				\
		void Invoke Params override														\
		{																				\
			(_Owner->*_Handler) Names ;													\
		}																				\
		void GetElement(void*& A, void*& B) override									\
		{																				\
			A = static_cast<void*>(_Owner);												\
			B = reinterpret_cast<void*>(*reinterpret_cast<int*>(&_Handler));					\
		}																				\
	};																					\
																						\
	class Name : public Object															\
	{																					\
	protected:																			\
		using HandlerList = std::vector<IEvent##Name*>;									\
		HandlerList _Handlers;															\
		int Find(IEvent##Name* Handler);												\
		void Bind(IEvent##Name* Handler);												\
		void Unbind(IEvent##Name* Handler);												\
	public:																				\
		Name();																			\
		~Name();																		\
		void Bind(EventNormal##Name::Handler Handler);									\
		void Unbind(EventNormal##Name::Handler Handler);								\
		template <typename TClass>														\
		void Bind(TClass* Owner, typename EventMember##Name<TClass>::Handler Handler)	\
		{																				\
			Bind(new EventMember##Name<TClass>(Owner, Handler));						\
		}																				\
		template <typename TClass>														\
		void Unbind(TClass* Owner, typename EventMember##Name<TClass>::Handler Handler)	\
		{																				\
			Unbind(new EventMember##Name<TClass>(Owner, Handler));						\
		}																				\
		void operator() Params;															\
	};

#define IMPLEMENT_VOID_EVENT(Name, Names, Params)										\
	EventNormal##Name::EventNormal##Name(Handler h)										\
		: _Handler(h)																	\
	{}																					\
	void EventNormal##Name::Invoke Params												\
	{																					\
		_Handler Names;																	\
	}																					\
	void EventNormal##Name::GetElement(void*& A, void*& B)								\
	{																					\
		A = nullptr;																	\
		B = static_cast<void*>(_Handler);												\
	}																					\
	int Name::Find(IEvent##Name* Handler)												\
	{																					\
		void* NewA;																		\
		void* NewB;																		\
		void* OldA;																		\
		void* OldB;																		\
		Handler->GetElement(NewA, NewB);												\
		for (size_t i = 0; i < _Handlers.size(); i++)									\
		{																				\
			_Handlers[i]->GetElement(OldA, OldB);										\
			if (NewA == OldA && NewB == OldB)											\
			{																			\
				return i;																\
			}																			\
		}																				\
		return -1;																		\
	}																					\
	void Name::Bind(IEvent##Name* Handler)												\
	{																					\
		int Index = Find(Handler);														\
		if (Index != -1)																\
		{																				\
			delete Handler;																\
		}																				\
		else																			\
		{																				\
			_Handlers.push_back(Handler);												\
		}																				\
	}																					\
	void Name::Unbind(IEvent##Name* Handler)											\
	{																					\
		int Index = Find(Handler);														\
		if (Index != -1)																\
		{																				\
			delete _Handlers[Index];													\
			_Handlers.erase(_Handlers.begin() + Index);									\
		}																				\
		delete Handler;																	\
	}																					\
	Name::Name(){}																		\
	Name::~Name()																		\
	{																					\
		for (auto& elem : _Handlers)													\
		{																				\
			delete elem;																\
		}																				\
	}																					\
	void Name::Bind(EventNormal##Name::Handler Handler)									\
	{																					\
		Bind(new EventNormal##Name(Handler));											\
	}																					\
	void Name::Unbind(EventNormal##Name::Handler Handler)								\
	{																					\
		Unbind(new EventNormal##Name(Handler));											\
	}																					\
	void Name::operator() Params														\
	{																					\
		for (size_t i = 0; i < _Handlers.size(); i++)									\
		{																				\
			_Handlers[i]->Invoke Names;													\
		}																				\
	}

}
#endif // EVENT_H
