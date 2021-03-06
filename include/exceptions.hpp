/*
 *  This file is part of the Xen Crashdump Analyser.
 *
 *  The Xen Crashdump Analyser is free software: you can redistribute
 *  it and/or modify it under the terms of the GNU General Public
 *  License as published by the Free Software Foundation, either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  The Xen Crashdump Analyser is distributed in the hope that it will
 *  be useful, but WITHOUT ANY WARRANTY; without even the implied
 *  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with the Xen Crashdump Analyser.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 *  Copyright (c) 2012 Citrix Inc.
 */

#ifndef __EXCEPTIONS_HPP__
#define __EXCEPTIONS_HPP__

/**
 * @file include/exceptions.hpp
 * @author Andrew Cooper
 */

#include <exception>
#include "types.hpp"
#include <stdlib.h>

/**
 * CommonError group of exceptions
 *
 * Base class for common exceptions.
 */
class CommonError: public std::exception
{
public:
    /// Constructor.
    CommonError() throw();

    /// Destructor.
    virtual ~CommonError() throw();

    /**
     * What is this exception.
     * @return string
     */
    virtual const char * what() const throw() = 0;

    /**
     * Log the error.
     */
    virtual void log() const throw() = 0;
};

/**
 * Memseek exception
 *
 * Results from failure to seek on /proc/vmcore, most likely because an attempt
 * is made to seek further than the 32bit kdump kernel can map.
 */
class memseek: public CommonError
{
public:
    /**
     * Constructor.
     * @param addr Intended address causing seek failure.
     * @param offset Offset into relevant memregion.
     */
    memseek(const maddr_t & addr, const int64_t & offset) throw();

    /// Destructor.
    virtual ~memseek() throw();

    /**
     * What is this exception.
     * @return string "memseek"
     */
    virtual const char * what() const throw();

    /**
     * Log the error.
     */
    virtual void log() const throw();

    /**
     * Is the address outside 64GB.
     * @return boolean
     */
    virtual bool outside_64GB() const throw();

    /// Intended address
    maddr_t addr;
    /// Offset into relevant memregion.
    int64_t offset;
};

/**
 * Memread exception
 *
 * Results from failure to read a set number of bytes from /proc/vmcore
 */
class memread: public CommonError
{
public:
    /**
     * Constructor.
     * @param addr Read location causing the failure.
     * @param count Number of bytes read.
     * @param total Intended number of bytes read.
     * @param error Error number (valid if count is -1).
     */
    memread(const maddr_t & addr, const ssize_t count, const ssize_t total, const int error) throw();

    /// Destructor.
    virtual ~memread() throw();

    /**
     * What is this exception.
     * @return string "memread"
     */
    virtual const char * what() const throw();

    /**
     * Log the error.
     */
    virtual void log() const throw();

    /**
     * Is the address outside 64GB.
     * @return boolean
     */
    virtual bool outside_64GB() const throw();

    /// Intended address
    maddr_t addr;
    /// Number of bytes read.
    ssize_t count;
    /// Intended number of bytes read.
    ssize_t total;
    /// Error (valid if count is -1)
    int error;
};

/**
 * Pagefault exception
 *
 * Results from failure to walk pagetables.
 */
class pagefault: public CommonError
{
public:
    /**
     * Constructor.
     * @param vaddr Faulting virtual address.
     * @param cr3 CR3 value used to start lookup.
     * @param level Which paging level caused the fault.
     */
    pagefault(const vaddr_t & vaddr, const uint64_t & cr3, const int level) throw();

    /// Destructor.
    virtual ~pagefault() throw();

    /**
     * What is this exception.
     * @return string "pagefault"
     */
    virtual const char * what() const throw();

    /**
     * Log the error.
     */
    virtual void log() const throw();

    /// Faulting virtual address.
    vaddr_t vaddr;
    /// Faulting cr3.
    uint64_t cr3;
    /// Faulting level.
    int level;
};

/**
 * Validate exception
 *
 * Thrown for validation failures for virtual addresses.
 */
class validate: public CommonError
{
public:
    /**
     * Constructor.
     * @param vaddr Faulting virtual address.
     * @param reason Reason for the fault.
     */
    validate(const vaddr_t & vaddr, const char * reason) throw();

    // This is needed to be public for throw(), but is never referenced.
    /// Copy constructor.
    validate(const validate &) throw();

    /// Destructor.
    virtual ~validate() throw();

    /**
     * What is this exception.
     * @return string "validate"
     */
    virtual const char * what() const throw();

    /**
     * Log the error.
     */
    virtual void log() const throw();

    /// Invalid virtual address.
    vaddr_t vaddr;
    /// Reason.
    const char * reason;

private:
    // @cond EXCLUDE
    validate & operator= (const validate &);
    // @endcond
};


/**
 * File write exception
 *
 * Thrown from FPRINTF (& friends) wrapper functions to catch and deal with
 * write errors to the stream.
 */
class filewrite: public std::exception
{
public:
    /**
     * Constructor.
     * @param error errno of the error.
     */
    filewrite(const int error) throw();

    /// Destructor.
    virtual ~filewrite() throw();

    /**
     * What is this exception.
     * @return string "validate"
     */
    virtual const char * what() const throw();

    /**
     * Log the error.
     * @param file Filename of the file being written to.
     */
    virtual void log(const char * file) const throw();

    /// Errno from error
    int error;
};

#endif

/*
 * Local variables:
 * mode: C++
 * c-file-style: "BSD"
 * c-basic-offset: 4
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
