#pragma once

#include "engine/core/base/variant.h"
#include "engine/core/math/Math.h"
#include "anim_curve.h"

namespace Echo
{
	class Node;
	struct AnimProperty
	{
		any							m_userData;					// name
		enum class Type
		{
			Unknow,
			Float,
			Vector3,
			Vector4,
			Quatition,
		}							 m_type;				// propert type
		AnimCurve::InterpolationType m_interpolationType;	// interpolation type

		// constuctor
		AnimProperty(Type type) : m_type(type) {}

		// create
		static AnimProperty* create(Type type);

		// set interpolation type
		virtual void setInterpolationType(AnimCurve::InterpolationType type);

		// optimize
		virtual void optimize() = 0;

		// update to time
		virtual void updateToTime(float time) = 0;

		// get length
		virtual float getLength() = 0;
	};

	struct AnimPropertyCurve : public AnimProperty
	{
		vector<AnimCurve*>::type m_curves;

		AnimPropertyCurve(Type type, i32 curveCount);

		// set interpolation type
		virtual void setInterpolationType(AnimCurve::InterpolationType type) override;

		// optimize
		virtual void optimize() override;

		// update to time
		virtual void updateToTime(float time) override{}

		// get length
		virtual float getLength() override;
	};

	struct AnimPropertyFloat : public AnimPropertyCurve
	{
		float		m_value;

		AnimPropertyFloat();

		// get value
		float getValue() { return m_value; }

		// add key
		void addKey(float time, const float value);

		// update to time
		virtual void updateToTime(float time) override;
	};

	struct AnimPropertyVec3 : public AnimPropertyCurve
	{
		Vector3		m_value;

		AnimPropertyVec3();

		// get value
		const Vector3& getValue() { return m_value; }

		// add key
		void addKey(float time, const Vector3& value);

		// update to time
		virtual void updateToTime(float time) override;
	};

	struct AnimPropertyVec4 : public AnimPropertyCurve
	{
		Vector4		m_value;

		AnimPropertyVec4();

		// get value
		const Vector4& getValue() { return m_value; }

		// add key
		void addKey(float time, const Vector4& value);

		// update to time
		virtual void updateToTime(float time) override;
	};

	struct AnimPropertyQuat : public AnimProperty
	{
		struct Key
		{
			float		m_time;
			Quaternion	m_value;
		};
		Quaternion				m_vlaue;
		vector<Key>::type		m_keys;

		AnimPropertyQuat() : AnimProperty(Type::Quatition), m_vlaue(Quaternion::IDENTITY) {}

		// get value
		const Quaternion& getValue() { return m_vlaue; }

		// add key
		void addKey(float time, const Quaternion& value);

		// optimize
		virtual void optimize() override {}

		// update to time
		virtual void updateToTime(float time) override;

		// get length
		virtual float getLength() override;
	};
}