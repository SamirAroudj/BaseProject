/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <string>
#include <iostream>

namespace FailureHandling
{
	/// This is the base class of all exceptions which are implemented in this project.
	/** This class is the base for all other exceptions implemented in this project.
        It facilitates track of situation specific problems by means of a message that
        can be stored and printed to a console window.*/
	class Exception
	{
	public:
		/**	Creates an exception and stores some information.
		@param message This variable should contain the reason for the exception creation.
		*/
		Exception(const std::string &message);

		/** Returns the reason why this object was created.
		@returns This is a message which explains the exceptional situation.
		*/
		const std::string &getMessage() const { return mMessage; }

		/** Returns the source. (birth place of the exception ;)
		@returns This param should contain the file and source code line in which the exception was created.
		*/
		const std::string &getSource() const { return mSource; }

	private:
        std::string mMessage;   /// A text which explains the reason and / or situation that caused this exception.
        std::string mSource;    /// Should contain the line and file which contain the exception's creation.
	};

	/** Exception's message and source can be printed to a console window.
	@param os This is the desired output stream, eg cout.
    @param exception This is the exception the message and source of which is printed. */
	std::ostream &operator<<(std::ostream &os, const Exception &exception);
}

#endif // _EXCEPTION_H_
