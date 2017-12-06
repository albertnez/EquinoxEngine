#pragma once
#include "FactoryUtils.h"
#include "BaseComponent.h"
#include <typeindex>

class BaseComponentEditor
{
public:
	BaseComponentEditor() = default;
	virtual ~BaseComponentEditor() = default;

	virtual std::type_index GetComponentTypeIndex() const = 0;

	virtual void DrawUI(BaseComponent* component) = 0;

	virtual bool IsRemovable() const
	{
		return true;
	}

	virtual bool CanBeDisabled() const
	{
		return true;
	}
};

CREATE_NAMED_STATIC_FACTORY(BaseComponentEditor, ComponentEditor)

#define REGISTER_COMPONENT_EDITOR(ClassName) \
	static ComponentEditorFactory<ClassName> __g_##ClassName##Factory;

#define DEFINE_COMPONENT_EDITOR(ComponentClass) \
	class ComponentClass##Editor : public BaseComponentEditor

#define BASE_COMPONENT_EDITOR(ComponentClass) \
	public: \
		std::type_index GetComponentTypeIndex() const override \
		{ \
			return typeid(ComponentClass); \
		} \
	private:
