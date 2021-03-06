#pragma once
#include <inttypes.h>
#include <blah/containers/str.h>
#include <blah/streams/endian.h>
#include <string.h>

namespace Blah
{
	class Stream
	{
	public:
		Stream() = default;
		Stream(const Stream&) = delete;
		Stream& operator=(const Stream&) = delete;

		virtual ~Stream() = default;

		// returns the length of the stream
		virtual int64_t length() const = 0;

		// returns the position of the stream
		virtual int64_t position() const = 0;

		// seeks the position of the stream
		virtual int64_t seek(int64_t seek_to) = 0;

		// returns true of the stream is open
		virtual bool is_open() const = 0;

		// returns true of the stream is readable
		virtual bool is_readable() const = 0;

		// returns true of the stream is writable
		virtual bool is_writable() const = 0;

		// closes the stream
		virtual void close() = 0;

		// pipes the contents of this tream to another stream
		int64_t pipe(Stream& to, int64_t length);

		// reads the amount of bytes into the given buffer, and returns the amount read
		int64_t read(void* buffer, int64_t length) { return read_into(buffer, length); }

		// reads a string. if length < 0, assumes null-terminated
		String read_string(int length = -1)
		{
			String result;

			if (length < 0)
			{
				char next;
				while (read(&next, 1) && next != '\0')
					result.append(next);
			}
			else
			{
				result.set_length(length);
				read_into(result.cstr(), length);
			}

			return result;
		}

		String read_line()
		{
			String result;

			char next;
			while (read(&next, 1) && next != '\n' && next != '\0')
				result.append(next);

			return result;
		}

		// reads a number
		template<class T>
		T read()
		{
			return read<T>(Endian::Little);
		}

		// reads a number
		template<class T>
		T read(Endian endian)
		{
			T result;
			read(&result, sizeof(T));
			if (!Blah::is_endian(endian))
				Blah::swap_endian(&result);
			return result;
		}

		// writes the amount of bytes to the stream from the given buffer, and returns the amount written
		int64_t write(const void* buffer, int64_t length) 
		{ 
			return write_from(buffer, length);
		}

		// writes a null-terminated string, and returns the amount written
		int64_t write_cstr(const Str& string)
		{
			return write(string.cstr(), string.length() + 1);
		}

		// writes a null-terminated string, and returns the amount written
		int64_t write_cstr(const char* cstr)
		{
			return write(cstr, strlen(cstr) + 1);
		}

		// writes a number
		template<class T>
		int64_t write(const T& value)
		{
			return write<T>(value, Endian::Little);
		}

		// writes a number
		template<class T>
		int64_t write(const T& value, Endian endian)
		{
			T writing = value;

			if (!Blah::is_endian(endian))
				Blah::swap_endian(&writing);

			return write(&writing, sizeof(T));
		}

	protected:
		// reads from the stream into the given buffer, and returns the number of bytes read
		virtual int64_t read_into(void* buffer, int64_t length) = 0;

		// writes from the stream from the given buffer, and returns the number of bytes written
		virtual int64_t write_from(const void* buffer, int64_t length) = 0;
	};
}

#undef BLAH_SWAP_ENDIAN
#undef BLAH_BIG_ENDIAN