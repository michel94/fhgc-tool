package it.unimi.dsi.law.warc.filters;

import it.unimi.dsi.law.bubing.util.BURL;
import it.unimi.dsi.law.warc.util.Response;

import com.google.common.base.Predicate;

/*		 
 * Copyright (C) 2012-2015 Paolo Boldi, Massimo Santini and Sebastiano Vigna 
 *
 *  This library is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License as published by the Free
 *  Software Foundation; either version 3 of the License, or (at your option)
 *  any later version.
 *
 *  This library is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 */

//RELEASE-STATUS: DIST

/** A filter is a strategy to decide whether to accept a given 
 *  object or not. Typically <code>T</code> will be either {@link BURL} or
 *  {@link Response}. Technically it is identical to the Google Guava 
 *  {@link Predicate} interface, but there are some conventions listed 
 *  below that apply only to filters.
 *  
 *  <p>By contract, every filter that is an instance of a non-anonymous
 *  filter class is supposed to have a <strong>static</strong>
 *  method with the following signature
 *  <pre>public static Filter&lt;T&gt; valueOf(String x)</pre>
 *  that returns a filter (typically, a filter of its own kind) from
 *  a string. Moreover, it is required, for every filter class <code>F</code>
 *  and for every instance <code>f</code>, that <code>toString()</code> returns
 *  <pre><var>classname</var>(<var>spec</var>)</pre> 
 *  where <pre>f.equals(F.valueOf(<var>spec</var>))</pre>
 *  
 *  <p>Note that <code><var>classname</var></code> can omit the package name if
 *  it is {@link #FILTER_PACKAGE_NAME}.
 */ 

public interface Filter<T> extends Predicate<T> {

	/** The name of the package that contains this interface as well as
	 *  most filters.
	 */
	public final static String FILTER_PACKAGE_NAME = Filter.class.getPackage().getName();
}
