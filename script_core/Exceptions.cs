using System;
using System.Runtime.Serialization;

namespace EveEngine
{
  /// <summary>
  /// Exception that will occur after trying to create multiple instances of a component.
  /// </summary>
  [Serializable]
  public class DuplicateComponentException : Exception
  {
    /// <summary>
    /// Initializes a new instance of the <see cref="DuplicateComponentException"/> class.
    /// </summary>
    public DuplicateComponentException() { }

    /// <summary>
    /// Initializes a new instance of the <see cref="DuplicateComponentException"/> class with a specified error message.
    /// </summary>
    /// <param name="message">The error message that explains the reason for the exception.</param>
    public DuplicateComponentException(string message) : base(message) { }

    /// <summary>
    /// Initializes a new instance of the <see cref="DuplicateComponentException"/> class with a specified error message
    /// and a reference to the inner exception that is the cause of this exception.
    /// </summary>
    /// <param name="message">The error message that explains the reason for the exception.</param>
    /// <param name="inner">The exception that is the cause of the current exception, or <c>null</c> if no inner exception is specified.</param>
    public DuplicateComponentException(string message, Exception inner) : base(message, inner) { }

    /// <summary>
    /// Initializes a new instance of the <see cref="DuplicateComponentException"/> class with serialized data.
    /// </summary>
    /// <param name="info">The <see cref="SerializationInfo"/> that holds the serialized object data about the exception being thrown.</param>
    /// <param name="context">The <see cref="StreamingContext"/> that contains contextual information about the source or destination.</param>
    protected DuplicateComponentException(SerializationInfo info, StreamingContext context) : base(info, context) { }
  }
}
