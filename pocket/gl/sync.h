#ifndef __POCKET_GL_SYNC_H__
#define __POCKET_GL_SYNC_H__

#include "../config.h"
#ifdef _USE_PRAGMA_ONCE
#pragma once
#endif // _USE_PRAGMA_ONCE

#include "gl.h"
#include "../io.h"

namespace pocket
{
namespace gl
{

// forward
class sync;

class sync
{
public:
	/*------------------------------------------------------------------------------------------
	* Types
	*------------------------------------------------------------------------------------------*/

	// コンストラクタでオブジェクトを取得
	enum construct_t
	{
		construct
	};
	// コマンドの送信を行なう
	enum flush_t
	{
		none = 0,
		flush = GL_SYNC_FLUSH_COMMANDS_BIT,
	};
	// waitでのステータス
	enum wait_status_t
	{
		// すでにGL命令が完了済み
		already_signaled = GL_ALREADY_SIGNALED,
		// タイムアウト前にシグナル状態
		conditional_satisfied = GL_CONDITION_SATISFIED,
		// タイムアウトした
		timeout = GL_TIMEOUT_EXPIRED,
		// 監視の失敗
		failed = GL_WAIT_FAILED,
		// 無効な値を入れた
		invalid_value = GL_INVALID_VALUE,
	};

private:
	/*------------------------------------------------------------------------------------------
	* Members
	*------------------------------------------------------------------------------------------*/

	GLsync _sync;
	GLenum _error_bitfield;

public:
	/*------------------------------------------------------------------------------------------
	* Constants
	*------------------------------------------------------------------------------------------*/

	// none

	/*------------------------------------------------------------------------------------------
	* Constructors
	*------------------------------------------------------------------------------------------*/

	sync() :
		_sync(NULL),
		_error_bitfield(0)
	{}
	explicit sync(construct_t) :
		_sync(glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0)),
		_error_bitfield(0)
	{
		if (_sync == NULL)
		{
			_error_bitfield |= error_creating;
		}
	}
	sync(const sync& s) :
		_sync(s._sync),
		_error_bitfield(0)
	{}
	~sync()
	{
		finalize();
	}

	/*------------------------------------------------------------------------------------------
	* Functions
	*------------------------------------------------------------------------------------------*/

	// 初期化
	bool initialize()
	{
		finalize();

		// フェンスオブジェクトの取得
		_sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
		if (_sync == NULL)
		{
			_error_bitfield |= error_creating;
			return false;
		}
		return true;
	}

	// 終了処理
	void finalize()
	{
		if (_sync != NULL)
		{
			glDeleteSync(_sync);
			_sync = NULL;
		}
		_error_bitfield = 0;
	}

	// GPU側がGPUの終了を待機
	// CPU側は何もしない
	void wait() const
	{
		glWaitSync(_sync, 0, GL_TIMEOUT_IGNORED);
	}

	// CPUでの待機
	// commitをsync::flushを指定でglFlush()と同じ効果を得られる
	void wait_client(GLuint64 timeout_nanoseconds, flush_t commit = none)
	{
		// エラー状態保持
		_error_bitfield = glClientWaitSync(_sync, commit, timeout_nanoseconds);
	}

	// シグナル状態か
	bool signaled() const
	{
		GLint signal = 0;
		glGetSynciv(_sync, GL_SYNC_STATUS, 1, NULL, &signal);
		return signal == GL_SIGNALED;
	}
	bool unsignaled() const
	{
		GLint signal = 0;
		glGetSynciv(_sync, GL_SYNC_STATUS, 1, NULL, &signal);
		return signal == GL_UNSIGNALED;
	}

	// エラー文
	std::string error() const
	{
		if (error_status(already_signaled))
		{
			return "failed. already signaled.";
		}
		if (error_status(conditional_satisfied))
		{
			return "failed. conditional satisfied.";
		}
		if (error_status(timeout))
		{
			return "failed. timeout";
		}
		if (error_status(failed))
		{
			return "failed. wait.";
		}
		if (error_status(invalid_value))
		{
			return "failed. invalid value.";
		}
		if (error_status(error_creating))
		{
			return "failed. glFenceSync()";
		}
		if (_sync == NULL)
		{
			return "not created. or already destroyed.";
		}
		// エラーは起こしていない
		return "";
	}

	// エラーのステータス確認
	bool error_status(error_bitfield bit) const
	{
		if (error_status(already_signaled) ||
			error_status(conditional_satisfied) ||
			error_status(timeout) ||
			error_status(failed) ||
			error_status(invalid_value))
		{
			return false;
		}
		return (_error_bitfield & bit) != 0;
	}
	bool error_status(wait_status_t e) const
	{
		return static_cast<wait_status_t>(_error_bitfield) == e;
	}

	// 有効な状態か
	bool valid() const
	{
		if (_sync == NULL ||
			_error_bitfield != 0)
		{
			return false;
		}
		return glIsSync(_sync) == GL_TRUE;
	}

	// ハンドルの取得
	GLsync& get()
	{
		return _sync;
	}
	const GLsync& get() const
	{
		return _sync;
	}

	/*------------------------------------------------------------------------------------------
	* Operators
	*------------------------------------------------------------------------------------------*/

	_CXX11_EXPLICIT operator bool () const
	{
		return valid();
	}
	bool operator ! () const
	{
		return !valid();
	}

	bool operator == (const sync& s) const
	{
		return _sync == s._sync;
	}
	bool operator != (const sync& s) const
	{
		return !(*this == s);
	}

	sync& operator = (const sync& s)
	{
		_sync = s._sync;
		_error_bitfield = s._error_bitfield;
		return *this;
	}
#ifdef _USE_CXX11
	sync& operator = (sync&& s)
	{
		_sync = std::move(s._sync);
		_error_bitfield = std::move(s._error_bitfield);
		s._sync = NULL;
		s._error_bitfield = 0;
		return *this;
	}

	sync& operator = (std::nullptr_t)
	{
		finalize();
		return *this;
	}
#endif // _USE_CXX11
};

template <typename CharT, typename CharTraits> inline
std::basic_ostream<CharT, CharTraits>& operator << (std::basic_ostream<CharT, CharTraits>& os, const sync& v)
{
	return os;
}

} // namespace gl
} // namespace pocket

#endif // __POCKET_GL_SYNC_H__
